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
#ifndef TITE_TYPE_TRAITS_ALLOCATOR_TAG_HPP
#define TITE_TYPE_TRAITS_ALLOCATOR_TAG_HPP

#include "tite/type_traits/void_t.hpp"

#include <type_traits>

namespace tite {

namespace detail {

template<typename T, typename = void>
struct is_allocator : std::false_type {};

template<typename T>
struct is_allocator<T, void_t<decltype(std::declval<T>().allocate(0))>> : std::true_type {};

template<typename T>
constexpr static bool is_allocator_v = is_allocator<T>::value;

} // namespace detail

template<typename T>
struct allocator_tag_t {
  static_assert(detail::is_allocator_v<T>, "T is not an allocator");
};

template<typename T>
constexpr static auto allocator_tag_v = allocator_tag_t<T>{};

namespace detail {

struct no_internal_allocator_type_t {};

constexpr static auto no_internal_allocator_type = no_internal_allocator_type_t{};

}

template<typename T, typename = void>
struct internal_allocator_type_tag {
  constexpr static auto value = detail::no_internal_allocator_type;
};

template<typename T>
struct internal_allocator_type_tag<T, tite::void_t<typename T::allocator_type>> {
  constexpr static auto value = allocator_tag_v<typename T::allocator_type>;
};

template<typename T>
constexpr static auto internal_allocator_type_tag_v = internal_allocator_type_tag<T>::value;

} // namespace tite

#endif
