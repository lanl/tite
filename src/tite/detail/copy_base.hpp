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
#ifndef TITE_DETAIL_COPY_BASE_HPP
#define TITE_DETAIL_COPY_BASE_HPP

#include "tite/detail/deep_copy_base.hpp"
#include "tite/detail/reference_counted_copy_base.hpp"
#include "tite/overload.hpp"
#include "tite/type_traits/all_of.hpp"

#include <type_traits>

namespace tite {

namespace detail {

struct copy_default_tag {};

template<typename CopyTagT, typename CRTPDerivedT, typename... OverloadTs>
struct copy_base;

template<typename CopyTagT, typename CRTPDerivedT, typename... OverloadTs>
using copy_base_t = typename copy_base<CopyTagT, CRTPDerivedT, OverloadTs...>::type;

template<typename CRTPDerivedT, typename... OverloadTs>
struct copy_base<copy_deep_tag, CRTPDerivedT, OverloadTs...> {
  using type = deep_copy_base<CRTPDerivedT>;
};

template<typename CRTPDerivedT, typename... OverloadTs>
struct copy_base<copy_reference_counted_tag, CRTPDerivedT, OverloadTs...> {
  using type = reference_counted_copy_base<CRTPDerivedT>;
};

template<typename CRTPDerivedT, typename... OverloadTs>
struct copy_base<copy_default_tag, CRTPDerivedT, OverloadTs...> {
  using type = std::conditional_t<tite::all_of_v<overload_references_const_self, OverloadTs...>,
                                  reference_counted_copy_base<CRTPDerivedT>,
                                  deep_copy_base<CRTPDerivedT> >;

};

} // namespace detail

} // namespace tite

#endif
