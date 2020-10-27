#include "tite.hpp"

#include "catch2/catch.hpp"

#include <type_traits>

struct get {
  template<typename T,
           typename IntT>
  friend constexpr auto tag_invoke(get, T&& t, IntT index)
  noexcept( noexcept( std::forward<T>(t)[index] ) )
  -> decltype( std::forward<T>(t)[index] )
  {
    return std::forward<T>(t)[index];
  }

  template<typename T, typename IntT>
  constexpr auto operator()(T&& t, IntT index) const
  noexcept( noexcept( tite::tag_invoke_fn{}(std::declval<get>(), std::forward<T>(t), index) ) )
  -> decltype( tite::tag_invoke_fn{}(std::declval<get>(), std::forward<T>(t), index) )
  {
    return tite::tag_invoke_fn{}(*this, std::forward<T>(t), index);
  }
};

TEST_CASE("any_unique",
          "[any_unique]") {

  using tite::any_unique;
  using tite::overload;
  using tite::self;

  any_unique< tite::default_memory_space,
              overload<get, int(self&, int)> > au1 = std::vector<int>{1,2,3,4,5};

  decltype(au1) au2{au1};

  decltype(au1) au3{au1};

  au3 = au2;

  CHECK( get{}(au1, 2) == 3 );
  CHECK( get{}(au2, 4) == 5 );
  CHECK( get{}(au3, 0) == 1 );
}
