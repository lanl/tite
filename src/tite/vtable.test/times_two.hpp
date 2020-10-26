#ifndef TITE_VTABLE_TEST_TIMES_TWO_HPP
#define TITE_VTABLE_TEST_TIMES_TWO_HPP

#include "tite.hpp"

struct times_two {
private:
  template<typename ArgT>
  friend constexpr auto tag_invoke(times_two, ArgT&& arg)
  noexcept
  -> decltype( 2*std::forward<ArgT>(arg) )
  {
    return 2 * std::forward<ArgT>(arg);
  }

public:
  template<typename ArgT>
  constexpr auto operator()(ArgT&& arg) const
  noexcept( noexcept( tite::tag_invoke_fn{}(std::declval<times_two const&>(), std::forward<ArgT>(arg)) ) )
  -> decltype( tite::tag_invoke_fn{}(std::declval<times_two const&>(), std::forward<ArgT>(arg)) )
  {
    return tite::tag_invoke_fn{}(*this, std::forward<ArgT>(arg));
  }
};

#endif
