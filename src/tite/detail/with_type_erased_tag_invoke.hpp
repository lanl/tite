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
  template<typename ObjectT = decltype(extract_self<ArgsT...>{}(std::declval<replace_self_t<ArgsT,DerivedT>>()...)),
           typename FnPtrT = decltype(std::declval<ObjectT&>().get_vtable()->template fn<OverloadT>())>
  friend ReturnT tag_invoke(base_cpo_t<OverloadT> cpo,
                            replace_self_t<ArgsT,DerivedT>... args)
  noexcept( noexcept( std::declval<FnPtrT>()(std::move(cpo), replace_self<ArgsT>::get((ArgsT&&) args, std::declval<void*&>())...) ) )
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
