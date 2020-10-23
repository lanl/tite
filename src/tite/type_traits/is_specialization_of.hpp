#ifndef TITE_TYPE_TRAITS_IS_SPECIALIZATION_OF_HPP
#define TITE_TYPE_TRAITS_IS_SPECIALIZATION_OF_HPP

#include <type_traits>

namespace tite {

template<template<typename...> class T, typename MaybeT>
struct is_specialization_of : std::false_type {};

template<template<typename...> class T, typename... ArgsT>
struct is_specialization_of<T, T<ArgsT...>> : std::true_type {};

template<template<typename...> class T, typename MaybeT>
static constexpr bool is_specialization_of_v = is_specialization_of<T, MaybeT>::value;

} // namespace tite

#endif
