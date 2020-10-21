#include "tite.hpp"
#include "tite/vtable/cuda_invoke_pointer_policy.hpp"

#include "times_two.hpp"

#include "cuda.h"

#include <cstdio>

template<typename VTableEntryT>
__global__ void call(VTableEntryT vte, int* const val)
{
  void* vptr = val;
  *val = vte.fn()(times_two{}, vptr);
}

int run_vtable_entry_cuda_test() {
  using tite::detail::vtable_entry;

  auto vte = vtable_entry<times_two, int(tite::self)>::construct<int,tite::cuda_invoke_pointer_policy>();

  int hint = 5;
  int* dint;
  cudaMalloc(&dint, sizeof(int));
  cudaMemcpy(dint, &hint, sizeof(int), cudaMemcpyHostToDevice);

  call<<<1,1>>>(vte, dint);
  cudaDeviceSynchronize();

  cudaMemcpy(&hint, dint, sizeof(int), cudaMemcpyDeviceToHost);

  return hint;
}
