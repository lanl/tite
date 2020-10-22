#include "tite.hpp"
#include "times_two.hpp"

#include "catch2/catch.hpp"

TEST_CASE("vtable",
          "[vtable]") {

  using tite::detail::vtable;
  using tite::overload;

  using times_two_ovld = overload<times_two, int(tite::self)>;

  auto vt = vtable<
              times_two_ovld>::construct<int>();

  int i = 5;
  void* vptr = &i;
  CHECK( vt.template fn<times_two_ovld>()(times_two_ovld{}, vptr) == 10 );
}
