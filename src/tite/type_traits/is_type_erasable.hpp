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
#ifndef TITE_TYPE_TRAITS_IS_TYPE_ERASABLE_HPP
#define TITE_TYPE_TRAITS_IS_TYPE_ERASABLE_HPP

#include "tite/type_traits/all_of.hpp"
#include "tite/type_traits/is_cpo_invocable.hpp"

namespace tite {

template<typename T, typename... OverloadsT>
struct is_type_erasable {
private:
  template<typename OverloadT>
  using this_one_is_type_erasable = is_cpo_invocable<T,OverloadT>;

public:
  static constexpr bool value = all_of_v<this_one_is_type_erasable, OverloadsT...>;
};

template<typename T, typename... OverloadsT>
static constexpr bool is_type_erasable_v = is_type_erasable<T, OverloadsT...>::value;

}

#endif
