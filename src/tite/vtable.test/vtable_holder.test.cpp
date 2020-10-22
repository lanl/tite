#include "tite.hpp"
#include "times_two.hpp"

#include "catch2/catch.hpp"

TEST_CASE("vtable_holder",
          "[vtable_holder]") {

  using tite::overload;

  SECTION("indirect_vtable_holder") {
    using tite::detail::vtable;
    using tite::detail::indirect_vtable_holder;
    using times_two_ovld = overload<times_two, int(tite::self)>;

    auto vth = indirect_vtable_holder<
                 std::allocator<vtable<times_two_ovld>>,
                 times_two_ovld>::construct<int>(std::allocator<int>{});

    int i = 5;
    void* vptr = &i;
    CHECK( vth->fn<times_two_ovld>()(times_two_ovld{}, vptr) == 10 );

    SECTION("test copy ctor") {
      indirect_vtable_holder<
        std::allocator<vtable<times_two_ovld>>,
        times_two_ovld> other_vth{vth};

      int i = 5;
      void* vptr = &i;
      CHECK( other_vth->fn<times_two_ovld>()(times_two_ovld{}, vptr) == 10 );
    }

    SECTION("test move ctor") {
      indirect_vtable_holder<
        std::allocator<vtable<times_two_ovld>>,
        times_two_ovld> other_vth{std::move(vth)};

      int i = 5;
      void* vptr = &i;
      CHECK( other_vth->fn<times_two_ovld>()(times_two_ovld{}, vptr) == 10 );
    }

    SECTION("test copy asgn") {
      auto other_vth = indirect_vtable_holder<
                         std::allocator<vtable<times_two_ovld>>,
                         times_two_ovld>::construct<int>(std::allocator<int>{});

      other_vth = vth;

      int i = 5;
      void* vptr = &i;
      CHECK( other_vth->fn<times_two_ovld>()(times_two_ovld{}, vptr) == 10 );
    }

    SECTION("test move asgn") {
      auto other_vth = indirect_vtable_holder<
                         std::allocator<vtable<times_two_ovld>>,
                         times_two_ovld>::construct<int>(std::allocator<int>{});

      other_vth = std::move(vth);

      int i = 5;
      void* vptr = &i;
      CHECK( other_vth->fn<times_two_ovld>()(times_two_ovld{}, vptr) == 10 );
    }
  }
}
