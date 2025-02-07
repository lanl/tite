#include "tite/tite.hpp"

#include "times_two.hpp"

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
  TITE_GPU_ASSERT(titeGPUMalloc(&dint, sizeof(int)));
  TITE_GPU_ASSERT(titeGPUMemcpy(dint, &hint, sizeof(int), titeGPUMemcpyHostToDevice));

  vtable_entry_test_kernel<<<1,1>>>(dint);
  TITE_GPU_ASSERT(titeGPUDeviceSynchronize());

  TITE_GPU_ASSERT(titeGPUMemcpy(&hint, dint, sizeof(int), titeGPUMemcpyDeviceToHost));
  TITE_GPU_ASSERT(titeGPUDeviceSynchronize());

  return hint;
}
