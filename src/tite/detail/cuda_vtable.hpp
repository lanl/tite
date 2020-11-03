#ifndef TITE_DETAIL_CUDA_VTABLE_HPP
#define TITE_DETAIL_CUDA_VTABLE_HPP

#include "cuda.h"

namespace tite {

namespace detail {

template<typename T, typename... OverloadsT>
__constant__ static tite::detail::vtable<OverloadsT...> const cuda_vtable_for = tite::detail::vtable<OverloadsT...>::template construct<T>();
template<typename T, typename... OverloadsT>
__constant__ static tite::detail::vtable<OverloadsT...> const* cuda_vtable_ptr = &cuda_vtable_for<T,OverloadsT...>;

template<typename T, typename... OverloadsT>
struct cuda_vtable_pointer_policy{
  constexpr tite::detail::vtable<OverloadsT...> const*
  operator()()
  noexcept
  {
    tite::detail::vtable<OverloadsT...>* ptr = nullptr;
    cudaMemcpyFromSymbol(&ptr, cuda_vtable_ptr<T,OverloadsT...>, sizeof(cuda_vtable_ptr<T,OverloadsT...>));
    return ptr;
  }

  constexpr void
  operator()(tite::detail::vtable<OverloadsT...>& v)
  noexcept
  {
    cudaMemcpyFromSymbol(&v,
                         cuda_vtable_for<T,OverloadsT...>,
                         sizeof(tite::detail::vtable<OverloadsT...>));
  }
};

} // namespace detail

} // namespace tite

#endif
