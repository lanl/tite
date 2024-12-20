#include "tite.hpp"

#include "catch2/catch_test_macros.hpp"

struct foo {};
struct bar {};

struct test_cpo {
  using signature_t = int(tite::self const&);

  constexpr int operator()(foo const&) const noexcept;
  constexpr foo operator()(bar const&) const noexcept;
};

TEST_CASE("is_cpo_invocable",
          "[type_traits][is_cpo_invocable]") {

  using tite::is_cpo_invocable_v;

  CHECK( is_cpo_invocable_v<foo, test_cpo> ); // works
  CHECK( not is_cpo_invocable_v<double, test_cpo> ); // fails is_invocable
  CHECK( not is_cpo_invocable_v<bar, test_cpo> ); // fails result convertible
}
