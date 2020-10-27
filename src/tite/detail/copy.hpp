#ifndef TITE_DETAIL_COPY_HPP
#define TITE_DETAIL_COPY_HPP

#include "tite/self.hpp"
#include "tite/type_traits/requires.hpp"
#include "tite/type_traits/tag_invoke.hpp"

namespace tite {

namespace detail {

struct copy {
  using signature_t = void(self const&, void*&);

  template<typename T,
           TITE_REQUIRES(is_tag_invocable<copy, T&&, void*&>::value)>
  constexpr void operator()(T&& obj, void*& ptr) const
  noexcept
  {
    tag_invoke_fn{}(copy{}, std::forward<T>(obj), ptr);
  }
};

} // namespace detail

} // namespace tite

#endif
