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
