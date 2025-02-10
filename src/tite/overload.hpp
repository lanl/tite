#ifndef TITE_OVERLOAD_HPP
#define TITE_OVERLOAD_HPP

#include "tite/self.hpp"

#include <type_traits>

namespace tite {

template<typename CPOT,
         typename SignatureT>
struct overload : CPOT {
  constexpr overload() = default;
  constexpr overload(CPOT) noexcept {};

  using signature_t = SignatureT;
};

namespace detail {

template<typename CPOT>
struct base_cpo {
  using type = CPOT;
};

template<typename CPOT, typename SignatureT>
struct base_cpo<overload<CPOT,SignatureT>> {
  using type = CPOT;
};

template<typename CPOT>
using base_cpo_t = typename base_cpo<CPOT>::type;

template<typename OverloadT>
struct overload_references_const_self;

template<typename OverloadT>
constexpr static auto overload_references_const_self_v = overload_references_const_self<OverloadT>::value;

template<typename CPOT,
         typename ReturnT,
         typename FirstT,
         typename... RestTs>
struct overload_references_const_self<overload<CPOT, ReturnT(FirstT,RestTs...)>> {
  using nonreference_type = std::remove_reference_t<FirstT>;
  using nonreference_noncv_type = std::remove_cv_t<nonreference_type>;
  constexpr static auto value =   std::is_reference<FirstT>::value
                              and std::is_const<nonreference_type>::value
                              and std::is_same<self, nonreference_noncv_type>::value;
};

} // namespace detail

} // namespace tite

#endif
