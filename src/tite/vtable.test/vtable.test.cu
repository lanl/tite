#include "tite.hpp"

#include "times_two.hpp"

#include "cuda.h"

#include <cstdio>

template<typename T, typename... OverloadsT>
__device__ static tite::detail::vtable<OverloadsT...> cuda_vtable_for = tite::detail::vtable<OverloadsT...>::template construct<T>();

__global__ void vtable_test_kernel(int* const val)
{
  using tite::overload;
  using times_two_ovld = overload<times_two, int(tite::self)>;
  auto& vt = cuda_vtable_for<int, times_two_ovld>;
  void* vptr = val;
  *val = vt.template fn<times_two_ovld>()(times_two{}, vptr);
}

int run_vtable_cuda_test() {

  int hint = 5;
  int* dint;
  cudaMalloc(&dint, sizeof(int));
  cudaMemcpy(dint, &hint, sizeof(int), cudaMemcpyHostToDevice);

  vtable_test_kernel<<<1,1>>>(dint);
  cudaDeviceSynchronize();

  cudaMemcpy(&hint, dint, sizeof(int), cudaMemcpyDeviceToHost);
  cudaDeviceSynchronize();

  return hint;
}
