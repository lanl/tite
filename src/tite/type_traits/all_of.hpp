#ifndef TITE_TYPE_TRAITS_ALL_OF_HPP
#define TITE_TYPE_TRAITS_ALL_OF_HPP

namespace tite {

template<template<typename> class Predicate, typename FirstT, typename... RestT>
struct all_of {
  constexpr static bool value = Predicate<FirstT>::value and all_of<Predicate, RestT...>::value;
};

template<template<typename> class Predicate, typename OnlyT>
struct all_of<Predicate, OnlyT> {
  constexpr static bool value = Predicate<OnlyT>::value;
};

template<template<typename> class Predicate, typename... TypesT>
constexpr static bool all_of_v = all_of<Predicate, TypesT...>::value;

} // namespace tite

#endif
