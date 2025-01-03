#include "tite.hpp"
#include "tite/detail/gpu_functions.hpp"

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

int run_vtable_entry_gpu_test() {

  int hint = 5;
  int* dint;
  titeGPUMalloc(&dint, sizeof(int));
  titeGPUMemcpy(dint, &hint, sizeof(int), titeGPUMemcpyHostToDevice);

  vtable_entry_test_kernel<<<1,1>>>(dint);
  titeGPUDeviceSynchronize();

  titeGPUMemcpy(&hint, dint, sizeof(int), titeGPUMemcpyDeviceToHost);
  titeGPUDeviceSynchronize();

  return hint;
}
