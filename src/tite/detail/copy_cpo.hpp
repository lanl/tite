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
#ifndef TITE_DETAIL_COPY_HPP
#define TITE_DETAIL_COPY_HPP

#include "tite/self.hpp"
#include "tite/type_traits/requires.hpp"
#include "tite/type_traits/tag_invoke.hpp"

namespace tite {

namespace detail {

struct copy_cpo {
  using signature_t = void(self const&, void*&);

  template<typename T,
           TITE_REQUIRES(is_tag_invocable<copy_cpo, T const&, void*&>::value)>
  constexpr void operator()(T const& obj, void*& ptr) const
  noexcept
  {
    tag_invoke_fn{}(copy_cpo{}, obj, ptr);
  }
};

} // namespace detail

} // namespace tite

#endif
