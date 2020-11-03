#include "tite.hpp"
#include "tite/detail/cuda_vtable.hpp"

#include "times_two.hpp"

#include "cuda.h"

#include <cstdio>

namespace cuda_vtable_holder_test {
  using tite::overload;
  using times_two_ovld = overload<times_two, int(tite::self)>;
}

template<typename VTableHolderT>
__global__ void vtable_holder_test_kernel(VTableHolderT vth, int* const val)
{
  using cuda_vtable_holder_test::times_two_ovld;

  void* vptr = val;
  *val = vth->template fn<times_two_ovld>()(times_two{}, vptr);
}

int run_indirect_vtable_holder_cuda_test() {
  int hint = 5;
  int* dint;
  cudaMalloc(&dint, sizeof(int));
  cudaMemcpy(dint, &hint, sizeof(int), cudaMemcpyHostToDevice);

  using cuda_vtable_holder_test::times_two_ovld;
  using tite::detail::indirect_vtable_holder;
  using tite::detail::cuda_vtable_pointer_policy;
  auto ivth = indirect_vtable_holder<times_two_ovld>::template construct<int,cuda_vtable_pointer_policy>();

  vtable_holder_test_kernel<<<1,1>>>(ivth, dint);
  cudaDeviceSynchronize();

  cudaMemcpy(&hint, dint, sizeof(int), cudaMemcpyDeviceToHost);
  cudaDeviceSynchronize();

  return hint;
}

int run_direct_vtable_holder_cuda_test() {
  int hint = 5;
  int* dint;
  cudaMalloc(&dint, sizeof(int));
  cudaMemcpy(dint, &hint, sizeof(int), cudaMemcpyHostToDevice);

  using cuda_vtable_holder_test::times_two_ovld;
  using tite::detail::direct_vtable_holder;
  using tite::detail::cuda_vtable_pointer_policy;
  auto dvth = direct_vtable_holder<times_two_ovld>::template construct<int,cuda_vtable_pointer_policy>();

  vtable_holder_test_kernel<<<1,1>>>(dvth, dint);
  cudaDeviceSynchronize();

  cudaMemcpy(&hint, dint, sizeof(int), cudaMemcpyDeviceToHost);
  cudaDeviceSynchronize();

  return hint;
}
