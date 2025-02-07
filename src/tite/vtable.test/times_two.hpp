#ifndef TITE_VTABLE_TEST_TIMES_TWO_HPP
#define TITE_VTABLE_TEST_TIMES_TWO_HPP

#include "tite/tite.hpp"

struct times_two : tite::cpo_base<times_two> {
private:
  template<typename ArgT>
  friend constexpr auto tag_invoke(times_two, ArgT&& arg)
  noexcept
  -> decltype( 2*std::forward<ArgT>(arg) )
  {
    return 2 * std::forward<ArgT>(arg);
  }
};

#endif
