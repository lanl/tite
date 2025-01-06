#ifndef TITE_DETAIL_GPU_VTABLE_HPP
#define TITE_DETAIL_GPU_VTABLE_HPP

namespace tite {

namespace detail {

template<typename T, typename... OverloadsT>
__constant__ inline tite::detail::vtable<OverloadsT...> const gpu_vtable_for = tite::detail::vtable<OverloadsT...>::template construct<T>();
template<typename T, typename... OverloadsT>
__constant__ inline tite::detail::vtable<OverloadsT...> const * const gpu_vtable_ptr = &gpu_vtable_for<T,OverloadsT...>;

} // namespace detail

} // namespace tite

#endif
