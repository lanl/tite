#include "tite/tite.hpp"

#include "catch2/catch_test_macros.hpp"

#include <type_traits>

struct foo {};
struct bar {};
struct baz {};

TEST_CASE("overload",
          "[overload]") {

  using tite::self;
  using tite::overload;
  using tite::detail::overload_references_const_self_v;

  static_assert(overload_references_const_self_v<overload<foo, bar const&(self const&)>>, "");
  static_assert(overload_references_const_self_v<overload<foo, bar const&(self const&&)>>, "");
  static_assert(not overload_references_const_self_v<overload<foo, bar const&(self&)>>, "");
  static_assert(not overload_references_const_self_v<overload<foo, bar const&(self&&)>>, "");

  SECTION("verify basic overload base and signature") {
    overload<foo, bar(baz)> ovld;

    CHECK( std::is_base_of<foo, decltype(ovld)>::value );
    CHECK( std::is_same<decltype(ovld)::signature_t, bar(baz)>::value );

    CHECK( std::is_same<tite::detail::base_cpo_t<decltype(ovld)>,foo>::value );
  }

  SECTION("verify overload base and signature including self") {
    overload<foo, bar const&(self const&, baz* const)> ovld;

    CHECK( std::is_base_of<foo, decltype(ovld)>::value );
    CHECK( std::is_same<decltype(ovld)::signature_t, bar const&(self const&, baz* const)>::value );

    CHECK( std::is_same<tite::detail::base_cpo_t<decltype(ovld)>,foo>::value );
  }
}
