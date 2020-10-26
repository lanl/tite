#include "tite.hpp"

#include "catch2/catch.hpp"
#include "tite/type_traits/invocable.hpp"
#include "tite/type_traits/tag_invoke.hpp"

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
  noexcept( noexcept( tite::tag_invoke_fn{}(*this, std::forward<T>(t), index) ) )
  -> decltype( tite::tag_invoke_fn{}(*this, std::forward<T>(t), index) )
  {
    return tite::tag_invoke_fn{}(*this, std::forward<T>(t), index);
  }
};

TEST_CASE("any_unique",
          "[any_unique]") {

  using tite::any_unique;
  using tite::overload;
  using tite::self;

  any_unique<std::allocator<unsigned char>> au = std::vector<int>{1,2,3,4,5};
  // any_unique< overload<get, int(self, int)> > au = std::vector<int>{1,2,3,4,5};

  // CHECK( get{}(au, 2) == 3 );
}
