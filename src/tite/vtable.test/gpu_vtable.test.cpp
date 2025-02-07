#include "tite/tite.hpp"

#include "times_two.hpp"

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
  TITE_GPU_ASSERT( titeGPUMalloc(&dint, sizeof(int)) );
  TITE_GPU_ASSERT( titeGPUMemcpy(dint, &hint, sizeof(int), titeGPUMemcpyHostToDevice) );

  vtable_test_kernel<<<1,1>>>(dint);
  TITE_GPU_ASSERT( titeGPUDeviceSynchronize() );

  TITE_GPU_ASSERT( titeGPUMemcpy(&hint, dint, sizeof(int), titeGPUMemcpyDeviceToHost) );
  TITE_GPU_ASSERT( titeGPUDeviceSynchronize() );

  return hint;
}
