#include "tite.hpp"
#include "tite/detail/gpu_functions.hpp"
#include "tite/detail/gpu_vtable.hpp"

#include "times_two.hpp"

#include <cstdio>

__global__ void vtable_test_kernel(int* const val)
{
  using tite::overload;
  using tite::detail::gpu_vtable_for;

  using times_two_ovld = overload<times_two, int(tite::self)>;

  auto& vt = gpu_vtable_for<int, times_two_ovld>;
  void* vptr = val;
  *val = vt.template fn<times_two_ovld>()(times_two{}, vptr);
}

int run_vtable_gpu_test() {

  int hint = 5;
  int* dint;
  titeGPUMalloc(&dint, sizeof(int));
  titeGPUMemcpy(dint, &hint, sizeof(int), titeGPUMemcpyHostToDevice);

  vtable_test_kernel<<<1,1>>>(dint);
  titeGPUDeviceSynchronize();

  titeGPUMemcpy(&hint, dint, sizeof(int), titeGPUMemcpyDeviceToHost);
  titeGPUDeviceSynchronize();

  return hint;
}
