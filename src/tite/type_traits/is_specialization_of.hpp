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
