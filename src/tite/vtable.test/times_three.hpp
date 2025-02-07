#ifndef TITE_VTABLE_TEST_TIMES_THREE_HPP
#define TITE_VTABLE_TEST_TIMES_THREE_HPP

#include "tite/tite.hpp"

struct times_three : tite::cpo_base<times_three> {
private:
  template<typename ArgT>
  friend constexpr auto tag_invoke(times_three, ArgT&& arg)
  noexcept
  -> decltype( 3*std::forward<ArgT>(arg) )
  {
    return 3 * std::forward<ArgT>(arg);
  }
};

#endif
