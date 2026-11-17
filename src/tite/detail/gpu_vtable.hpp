/*
 * © 2025. Triad National Security, LLC. All rights reserved.
 * This program was produced under U.S. Government contract 89233218CNA000001
 * for Los Alamos National Laboratory (LANL), which is operated by Triad
 * National Security, LLC for the U.S. Department of Energy/National Nuclear
 * Security Administration. All rights in the program are reserved by Triad
 * National Security, LLC, and the U.S. Department of Energy/National Nuclear
 * Security Administration. The Government is granted for itself and others
 * acting on its behalf a nonexclusive, paid-up, irrevocable worldwide license
 * in this material to reproduce, prepare derivative works, distribute copies
 * to the public, perform publicly and display publicly, and to permit others
 * to do so.
 */
#ifndef TITE_DETAIL_GPU_VTABLE_HPP
#define TITE_DETAIL_GPU_VTABLE_HPP

namespace tite {

namespace detail {

template<typename T, typename... OverloadsT>
__device__ static tite::detail::vtable<OverloadsT...> const gpu_vtable_for = tite::detail::vtable<OverloadsT...>::template construct<T>();

template<typename T, typename... OverloadsT>
__device__ static tite::detail::vtable<OverloadsT...> const * const gpu_vtable_ptr = &gpu_vtable_for<T,OverloadsT...>;

} // namespace detail

} // namespace tite

#endif
