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
