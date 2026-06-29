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
#ifndef TITE_ANY_UNIQUE_TEST_GET_CPO_HPP
#define TITE_ANY_UNIQUE_TEST_GET_CPO_HPP

#include "tite/tite.hpp"

#include <utility>

struct get_cpo : tite::cpo_base<get_cpo> {
private:
  template<typename T,
           typename IntT>
  friend constexpr auto tag_invoke(get_cpo, T&& t, IntT index)
  noexcept( noexcept( std::forward<T>(t)[index] ) )
  -> decltype( std::forward<T>(t)[index] )
  {
    return std::forward<T>(t)[index];
  }
};

TITE_CONSTEXPR_STATIC auto get = get_cpo{};

#endif
