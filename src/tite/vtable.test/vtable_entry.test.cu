#include "tite.hpp"
// #include "tite/cuda_invoke_pointer_policy.hpp"

#include "times_two.hpp"

#include "cuda.h"

#include <cstdio>

__global__ void vtable_entry_test_kernel(int* const val)
{
  using tite::detail::vtable_entry;
  auto vte = vtable_entry<times_two, int(tite::self)>::construct<int>();
  void* vptr = val;
  *val = vte.fn()(times_two{}, vptr);
}

int run_vtable_entry_cuda_test() {

  int hint = 5;
  int* dint;
  cudaMalloc(&dint, sizeof(int));
  cudaMemcpy(dint, &hint, sizeof(int), cudaMemcpyHostToDevice);

  vtable_entry_test_kernel<<<1,1>>>(dint);
  cudaDeviceSynchronize();

  cudaMemcpy(&hint, dint, sizeof(int), cudaMemcpyDeviceToHost);
  cudaDeviceSynchronize();

  return hint;
}
