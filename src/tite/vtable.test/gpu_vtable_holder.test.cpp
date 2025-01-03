#include "tite.hpp"
#include "tite/gpu_allocator.hpp"

#include "times_two.hpp"

#include <cstdio>

namespace gpu_vtable_holder_test {
  using tite::overload;
  using times_two_ovld = overload<times_two, int(tite::self)>;
}

template<typename VTableHolderT>
__global__ void vtable_holder_test_kernel(VTableHolderT vth, int* const val)
{
  using gpu_vtable_holder_test::times_two_ovld;

  void* vptr = val;
  *val = vth->template fn<times_two_ovld>()(times_two{}, vptr);
}

int run_indirect_vtable_holder_gpu_test() {
  int hint = 5;
  int* dint;
  titeGPUMalloc(&dint, sizeof(int));
  titeGPUMemcpy(dint, &hint, sizeof(int), titeGPUMemcpyHostToDevice);

  using gpu_vtable_holder_test::times_two_ovld;
  using tite::detail::indirect_vtable_holder;
  using tite::gpu_allocator;
  auto ivth = indirect_vtable_holder<times_two_ovld>::template construct<int>(gpu_allocator<int>{});

  vtable_holder_test_kernel<<<1,1>>>(ivth, dint);
  titeGPUDeviceSynchronize();

  titeGPUMemcpy(&hint, dint, sizeof(int), titeGPUMemcpyDeviceToHost);
  titeGPUDeviceSynchronize();

  return hint;
}

int run_direct_vtable_holder_gpu_test() {
  int hint = 5;
  int* dint;
  titeGPUMalloc(&dint, sizeof(int));
  titeGPUMemcpy(dint, &hint, sizeof(int), titeGPUMemcpyHostToDevice);

  using gpu_vtable_holder_test::times_two_ovld;
  using tite::detail::direct_vtable_holder;
  using tite::gpu_allocator;
  auto dvth = direct_vtable_holder<times_two_ovld>::template construct<int>(gpu_allocator<int>{});

  vtable_holder_test_kernel<<<1,1>>>(dvth, dint);
  titeGPUDeviceSynchronize();

  titeGPUMemcpy(&hint, dint, sizeof(int), titeGPUMemcpyDeviceToHost);
  titeGPUDeviceSynchronize();

  return hint;
}
