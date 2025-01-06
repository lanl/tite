#ifndef TITE_TYPE_TRAITS_ALLOCATOR_TAG_HPP
#define TITE_TYPE_TRAITS_ALLOCATOR_TAG_HPP

#include "tite/type_traits/void_t.hpp"

#include <type_traits>

namespace tite {

namespace detail {

struct null_allocator_tag_t {};

constexpr static auto null_allocator_tag = null_allocator_tag_t{};

}

template<typename T>
struct allocator_tag_t {};

template<typename T>
constexpr static auto allocator_tag = allocator_tag_t<T>{};

template<typename T, typename = void>
struct has_allocator_type : std::false_type {};

template<typename T>
struct has_allocator_type<T, void_t<typename T::allocator_type>> : std::true_type {};

template<typename T>
constexpr static bool has_allocator_type_v = has_allocator_type<T>::value;

template<typename T, bool = has_allocator_type_v<T>>
struct get_allocator_tag;

template<typename T>
struct get_allocator_tag<T, true> {
  constexpr static auto value = allocator_tag<typename T::allocator_type>;
};

template<typename T>
struct get_allocator_tag<T, false> {
  constexpr static auto value = detail::null_allocator_tag;
};

template<typename T>
constexpr static auto get_allocator_tag_v = get_allocator_tag<T>::value;

} // namespace tite

#endif
