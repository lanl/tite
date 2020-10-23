#ifndef TITE_CUDA_INVOKE_POINTER_POLICY_HPP
#define TITE_CUDA_INVOKE_POINTER_POLICY_HPP

#ifndef __CUDACC__
#error "cuda_invoke_pointer_policy.hpp can only be included in CUDA compilations"
#endif

#include "tite/self.hpp"
#include "tite/detail/vtable_invoke.hpp"

#include "cuda.h"
#include <type_traits>

namespace tite {

namespace detail {

template<typename T, typename CPOT, typename ReturnT, typename... ArgsT>
using cuda_invoke_function_t = ReturnT(CPOT,ArgsT...);

template<typename T, typename CPOT, typename ReturnT, typename... ArgsT>
__device__ cuda_invoke_function_t<T,CPOT,ReturnT,tite::replace_self_with_void_ptr_t<ArgsT>...>* cuda_invoke_ptr =
  tite::detail::vtable_invoke<T,CPOT,ReturnT,ArgsT...>;

} // namespace detail

template<typename T, typename CPOT, typename ReturnT, typename... ArgsT>
struct cuda_invoke_pointer_policy {
  template<typename FunctionT>
  constexpr void operator()(FunctionT*& ptr) const {
    cudaMemcpyFromSymbol(&ptr, detail::cuda_invoke_ptr<T,CPOT,ReturnT,ArgsT...>, sizeof(FunctionT*));
    cudaDeviceSynchronize(); // is this needed ???
  }
};

} // namespace tite

#endif
