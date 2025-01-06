#ifndef TITE_DETAIL_DEALLOCATE_HPP
#define TITE_DETAIL_DEALLOCATE_HPP

#include "tite/detail/gpu_functions.hpp"
#include "tite/self.hpp"
#include "tite/type_traits/requires.hpp"
#include "tite/type_traits/tag_invoke.hpp"

namespace tite {

namespace detail {


struct deallocate {
  using signature_t = void(self&&);

  template<typename T,
           TITE_REQUIRES(is_tag_invocable<deallocate, T&&>::value)>
  constexpr void operator()(T&& obj) const
  noexcept
  {
#if ! TITE_DEVICE_COMPILE()
    tag_invoke_fn{}(deallocate{}, std::forward<T>(obj));
#endif
  }
};

} // namespace detail

} // namespace tite

#endif
