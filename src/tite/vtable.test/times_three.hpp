#ifndef TITE_VTABLE_TEST_TIMES_THREE_HPP
#define TITE_VTABLE_TEST_TIMES_THREE_HPP

#include "tite.hpp"

struct times_three {
private:
  template<typename ArgT>
  friend constexpr auto tag_invoke(times_three, ArgT&& arg)
  noexcept
  -> decltype( 3*std::forward<ArgT>(arg) )
  {
    return 3 * std::forward<ArgT>(arg);
  }

public:
  template<typename ArgT>
  constexpr auto operator()(ArgT&& arg) const
  noexcept( noexcept( tite::tag_invoke_fn{}(std::declval<times_three const&>(), std::forward<ArgT>(arg)) ) )
  -> decltype( tite::tag_invoke_fn{}(std::declval<times_three const&>(), std::forward<ArgT>(arg)) )
  {
    return tite::tag_invoke_fn{}(*this, std::forward<ArgT>(arg));
  }
};

#endif
