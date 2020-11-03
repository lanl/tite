#include "tite.hpp"

#include "times_two.hpp"

#include "cuda.h"

#include <cstdio>

template<typename T, typename... OverloadsT>
__device__ static tite::detail::vtable<OverloadsT...> const cuda_vtable_for = tite::detail::vtable<OverloadsT...>::template construct<T>();
template<typename T, typename... OverloadsT>
__device__ static tite::detail::vtable<OverloadsT...> const* cuda_vtable_ptr = &cuda_vtable_for<T,OverloadsT...>;

template<typename T, typename... OverloadsT>
struct CudaVTablePointerPolicy {
  constexpr tite::detail::vtable<OverloadsT...> const*
  operator()()
  noexcept
  {
    tite::detail::vtable<OverloadsT...>* ptr = nullptr; 
    cudaMemcpyFromSymbol(&ptr, cuda_vtable_ptr<T,OverloadsT...>, sizeof(cuda_vtable_ptr<T,OverloadsT...>));
    return ptr;
  }
};

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
  auto ivth = indirect_vtable_holder<times_two_ovld>::template construct<int,CudaVTablePointerPolicy>();

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
  auto dvth = direct_vtable_holder<times_two_ovld>::template construct<int,CudaVTablePointerPolicy>();

  vtable_holder_test_kernel<<<1,1>>>(dvth, dint);
  cudaDeviceSynchronize();

  cudaMemcpy(&hint, dint, sizeof(int), cudaMemcpyDeviceToHost);
  cudaDeviceSynchronize();

  return hint;
}
