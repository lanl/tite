#ifndef TITE_ANY_UNIQUE_TEST_GET_CPO_HPP
#define TITE_ANY_UNIQUE_TEST_GET_CPO_HPP

#include "tite/tite.hpp"

#include <utility>

struct get_cpo : tite::cpo_base<get_cpo> {
private:
  template<typename T,
           typename IntT>
  friend constexpr auto tag_invoke(get_cpo, T&& t, IntT index)
  noexcept( noexcept( std::forward<T>(t)[index] ) )
  -> decltype( std::forward<T>(t)[index] )
  {
    return std::forward<T>(t)[index];
  }
};

TITE_CONSTEXPR_STATIC auto get = get_cpo{};

#endif
