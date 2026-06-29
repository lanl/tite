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
#ifndef TITE_DETAIL_WITH_TYPE_ERASED_TAG_INVOKE_HPP
#define TITE_DETAIL_WITH_TYPE_ERASED_TAG_INVOKE_HPP

#include "tite/overload.hpp"
#include "tite/self.hpp"

namespace tite {

namespace detail {

template<typename DerivedT,
         typename OverloadT,
         typename SignatureT = typename OverloadT::signature_t>
struct with_type_erased_tag_invoke;

template<typename DerivedT,
         typename OverloadT,
         typename ReturnT,
         typename... ArgsT>
struct with_type_erased_tag_invoke<DerivedT, OverloadT, ReturnT(ArgsT...)> {
  // template<typename ObjectT = decltype(extract_self<ArgsT...>{}(std::declval<replace_self_t<ArgsT,DerivedT>>()...)),
  //          typename FnPtrT = decltype(std::declval<ObjectT&>().get_vtable()->template fn<OverloadT>())>
  constexpr
  friend ReturnT tag_invoke(base_cpo_t<OverloadT> cpo,
                            replace_self_t<ArgsT,DerivedT>... args)
  // noexcept( noexcept( std::declval<FnPtrT>()(std::move(cpo), replace_self<ArgsT>::get((ArgsT&&) args, std::declval<void*&>())...) ) )
  {
    auto& t = extract_self<ArgsT...>{}(args...);
    void* t_ptr = t.get_object_address();
    auto* fn_ptr = t.get_vtable()->template fn<OverloadT>();
    return fn_ptr(std::move(cpo), replace_self<ArgsT>::get((ArgsT&&) args, t_ptr)...);
  }
};

} // namespace detail

} // namespace tite

#endif
