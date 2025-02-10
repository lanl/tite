#ifndef TITE_DETAIL_COPY_BASE_HPP
#define TITE_DETAIL_COPY_BASE_HPP

#include "tite/detail/deep_copy_base.hpp"
#include "tite/detail/reference_counted_copy_base.hpp"
#include "tite/overload.hpp"
#include "tite/type_traits/all_of.hpp"

#include <type_traits>

namespace tite {

namespace detail {

template<typename CRTPDerivedT, typename... OverloadTs>
using copy_base = std::conditional_t<tite::all_of_v<overload_references_const_self, OverloadTs...>,
                                     reference_counted_copy_base<CRTPDerivedT>,
                                     deep_copy_base<CRTPDerivedT> >;

} // namespace detail

} // namespace tite

#endif
