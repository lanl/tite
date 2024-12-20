#ifndef TITE_OVERLOAD_HPP
#define TITE_OVERLOAD_HPP

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

} // namespace detail

} // namespace tite

#endif
