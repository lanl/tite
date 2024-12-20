#include "tite.hpp"
#include "times_two.hpp"

#include "catch2/catch_test_macros.hpp"

#ifdef TITE_ENABLE_CUDA
int run_indirect_vtable_holder_cuda_test();
int run_direct_vtable_holder_cuda_test();
#endif

TEST_CASE("vtable_holder",
          "[vtable_holder]") {

  using tite::overload;
  using times_two_ovld = overload<times_two, int(tite::self)>;

  SECTION("indirect_vtable_holder") {
    using tite::detail::indirect_vtable_holder;

    auto ivth = indirect_vtable_holder<times_two_ovld>::template construct<int>();

    int i = 5;
    void* vptr = &i;
    CHECK( ivth->template fn<times_two_ovld>()(times_two_ovld{}, vptr) == 10 );

#   ifdef TITE_ENABLE_CUDA
    CHECK( run_indirect_vtable_holder_cuda_test() == 10 );
#   endif
  }

  SECTION("direct_vtable_holder") {
    using tite::detail::direct_vtable_holder;

    auto dvth = direct_vtable_holder<times_two_ovld>::template construct<int>();

    int i = 5;
    void* vptr = &i;
    CHECK( dvth->template fn<times_two_ovld>()(times_two_ovld{}, vptr) == 10 );

#   ifdef TITE_ENABLE_CUDA
    CHECK( run_direct_vtable_holder_cuda_test() == 10 );
#   endif
  }
}
