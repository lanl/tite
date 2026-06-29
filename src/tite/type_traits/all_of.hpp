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
#ifndef TITE_TYPE_TRAITS_ALL_OF_HPP
#define TITE_TYPE_TRAITS_ALL_OF_HPP

namespace tite {

template<template<typename> class Predicate, typename... ArgsT>
struct all_of;

template<template<typename> class Predicate, typename FirstT, typename... RestT>
struct all_of<Predicate, FirstT, RestT...> {
  constexpr static bool value = Predicate<FirstT>::value and all_of<Predicate, RestT...>::value;
};

template<template<typename> class Predicate>
struct all_of<Predicate> {
  constexpr static bool value = true;
};

template<template<typename> class Predicate, typename... TypesT>
constexpr static bool all_of_v = all_of<Predicate, TypesT...>::value;

} // namespace tite

#endif
