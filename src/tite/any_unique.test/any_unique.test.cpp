#include "tite.hpp"

#include "catch2/catch_test_macros.hpp"

#include <type_traits>

#include <deque>

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

TEST_CASE("any_unique",
          "[any_unique]") {

  using tite::any_unique;
  using tite::overload;
  using tite::self;

  any_unique< overload<get_cpo, int&(self&, int)>,
              overload<get_cpo, int const&(self const&, int)> > const au1 = {std::deque<int>{1,2,3,4,5}, std::allocator<unsigned char>{}};

  decltype(au1) au2{au1};

  tite::remove_cvref_t<decltype(au1)> au3{au1};

  au3 = au2;

  get(au3,0) = 5;

  CHECK( get(au1, 2) == 3 );
  CHECK( get(au2, 4) == 5 );
  CHECK( get(au3, 0) == 5 );
}
