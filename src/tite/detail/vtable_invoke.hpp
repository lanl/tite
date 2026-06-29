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
#ifndef TITE_DETAIL_VTABLE_INVOKE_HPP
#define TITE_DETAIL_VTABLE_INVOKE_HPP

#include "tite/self.hpp"
#include "tite/type_traits/allocator_tag.hpp"
#include "tite/detail/void_ptr_cast.hpp"

namespace tite {

namespace detail {

template<typename T,
         typename CPOT,
         typename ReturnT,
         typename... ArgsT,
         TITE_REQUIRES(not std::is_same<ReturnT,void>::value)>
constexpr auto vtable_invoke(CPOT cpo,
                             replace_self_with_void_ptr_t<ArgsT>... args)
noexcept( noexcept( std::move(cpo)(replace_self<ArgsT>::get(std::forward<replace_self_with_void_ptr_t<ArgsT>>(args), std::declval<T&>())...) ) )
-> ReturnT
{
  void* self_ptr = extract_self<ArgsT...>{}(args...);
  T& obj = void_ptr_cast<T>(self_ptr, internal_allocator_type_tag_v<T>);
  return std::move(cpo)(replace_self<ArgsT>::get(std::forward<replace_self_with_void_ptr_t<ArgsT>>(args), obj)...);
}

template<typename T,
         typename CPOT,
         typename ReturnT,
         typename... ArgsT,
         TITE_REQUIRES(std::is_same<ReturnT,void>::value)>
constexpr void vtable_invoke(CPOT cpo,
                             replace_self_with_void_ptr_t<ArgsT>... args)
noexcept( noexcept( std::move(cpo)(replace_self<ArgsT>::get(std::forward<replace_self_with_void_ptr_t<ArgsT>>(args), std::declval<T&>())...) ) )
{
  void* self_ptr = extract_self<ArgsT...>{}(args...);
  T& obj = void_ptr_cast<T>(self_ptr, internal_allocator_type_tag_v<T>);
  std::move(cpo)(replace_self<ArgsT>::get(std::forward<replace_self_with_void_ptr_t<ArgsT>>(args), obj)...);
}
} // namespace detail

} // namespace tite

#endif
