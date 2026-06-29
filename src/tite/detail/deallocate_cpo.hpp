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
#ifndef TITE_DETAIL_DEALLOCATE_CPO_HPP
#define TITE_DETAIL_DEALLOCATE_CPO_HPP

#include "tite/detail/gpu_functions.hpp"
#include "tite/self.hpp"
#include "tite/type_traits/requires.hpp"
#include "tite/type_traits/tag_invoke.hpp"

namespace tite {

namespace detail {

struct deallocate_cpo {
  using signature_t = void(self&&);

  template<typename T,
           TITE_REQUIRES(is_tag_invocable<deallocate_cpo, T&&>::value)>
  constexpr void operator()(T&& obj) const
  noexcept
  {
#if ! TITE_DEVICE_COMPILE()
    tag_invoke_fn{}(deallocate_cpo{}, std::forward<T>(obj));
#endif
  }
};

} // namespace detail

} // namespace tite

#endif
