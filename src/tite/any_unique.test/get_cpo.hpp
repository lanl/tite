#ifndef TITE_ANY_UNIQUE_TEST_GET_CPO_HPP
#define TITE_ANY_UNIQUE_TEST_GET_CPO_HPP

#include "tite.hpp"

#include <utility>

struct get_cpo {
  template<typename T,
           typename IntT>
  friend constexpr auto tag_invoke(get_cpo, T&& t, IntT index)
  noexcept( noexcept( std::forward<T>(t)[index] ) )
  -> decltype( std::forward<T>(t)[index] )
  {
    return std::forward<T>(t)[index];
  }

  template<typename T, typename IntT>
  constexpr auto operator()(T&& t, IntT index) const
  noexcept( noexcept( tite::tag_invoke_fn{}(std::declval<get_cpo>(), std::forward<T>(t), index) ) )
  -> decltype( tite::tag_invoke_fn{}(std::declval<get_cpo>(), std::forward<T>(t), index) )
  {
    return tite::tag_invoke_fn{}(*this, std::forward<T>(t), index);
  }
};

constexpr static auto get = get_cpo{};

#endif
