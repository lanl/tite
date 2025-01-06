#ifndef TITE_DETAIL_WITH_FORWARDING_TAG_INVOKE_HPP
#define TITE_DETAIL_WITH_FORWARDING_TAG_INVOKE_HPP

#include "tite/overload.hpp"
#include "tite/self.hpp"

namespace tite {

namespace detail {

template<typename DerivedT,
         typename OverloadT,
         typename SignatureT = typename OverloadT::signature_t>
struct with_forwarding_tag_invoke;

template<typename DerivedT,
         typename OverloadT,
         typename ReturnT,
         typename... ArgsT>
struct with_forwarding_tag_invoke<DerivedT, OverloadT, ReturnT(ArgsT...)> {
  template<typename WrapperT = decltype(extract_self<ArgsT...>{}(std::declval<replace_self_t<ArgsT,DerivedT>>()...)),
           typename ObjectT = decltype(std::declval<WrapperT&>().value)>
  constexpr
  friend ReturnT tag_invoke(base_cpo_t<OverloadT> cpo,
                            replace_self_t<ArgsT,DerivedT>... args)
  // noexcept( noexcept( std::move(cpo)(replace_self<ArgsT>::get(std::forward<replace_self_t<ArgsT,DerivedT>&&>(args), std::declval<ObjectT&>())...) ) )
  {
    auto& wrapper = extract_self<ArgsT...>{}(args...);
    auto& t = wrapper.value;
    return std::move(cpo)(replace_self<ArgsT>::get(std::forward<replace_self_t<ArgsT,DerivedT>&&>(args), t)...);
  }
};

} // namespace detail

} // namespace tite

#endif
