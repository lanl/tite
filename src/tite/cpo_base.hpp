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
#ifndef TITE_CPO_BASE_HPP
#define TITE_CPO_BASE_HPP

#include "tite/type_traits/tag_invoke.hpp"

namespace tite {

template<typename CRTPDerivedT>
struct cpo_base {

template<typename... ArgTs>
constexpr auto operator()(ArgTs&&... args) const
noexcept( noexcept( tite::tag_invoke_fn{}(std::declval<CRTPDerivedT const&>(), std::forward<ArgTs>(args)...) ) )
-> decltype( tite::tag_invoke_fn{}(std::declval<CRTPDerivedT const&>(), std::forward<ArgTs>(args)...) )
{
  return tite::tag_invoke_fn{}(static_cast<CRTPDerivedT const&>(*this), std::forward<ArgTs>(args)...);
}

};

}

#endif
