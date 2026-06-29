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
#ifndef TITE_VTABLE_TEST_TIMES_THREE_HPP
#define TITE_VTABLE_TEST_TIMES_THREE_HPP

#include "tite/tite.hpp"

struct times_three : tite::cpo_base<times_three> {
private:
  template<typename ArgT>
  friend constexpr auto tag_invoke(times_three, ArgT&& arg)
  noexcept
  -> decltype( 3*std::forward<ArgT>(arg) )
  {
    return 3 * std::forward<ArgT>(arg);
  }
};

#endif
