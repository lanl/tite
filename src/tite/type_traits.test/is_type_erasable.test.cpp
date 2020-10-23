#include "tite.hpp"

#include "catch2/catch.hpp"

struct foo {};
struct bar {};
struct baz : bar {};

struct cpo1 {
  using signature_t = int(tite::self const&);

  constexpr int operator()(foo const&) const noexcept;
  constexpr foo operator()(bar const&) const noexcept;
  constexpr double operator()(baz const&) const noexcept;
};

struct cpo2 {
  using signature_t = bar(tite::self const&);

  constexpr baz operator()(foo const&) const noexcept;
  constexpr foo operator()(bar const&) const noexcept;
  constexpr bar operator()(baz const&) const noexcept;
};

TEST_CASE("is_type_erasable",
          "[type_traits][is_type_erasable]") {

  using tite::is_type_erasable_v;

  CHECK( is_type_erasable_v<foo, cpo1, cpo2> );
  CHECK( not is_type_erasable_v<bar, cpo1, cpo2> );
  CHECK( is_type_erasable_v<baz, cpo1, cpo2> );
}
