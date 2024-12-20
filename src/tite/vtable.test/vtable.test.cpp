#include "tite.hpp"
#include "times_two.hpp"

#include "catch2/catch_test_macros.hpp"

#ifdef TITE_ENABLE_CUDA
int run_vtable_cuda_test();
#endif

TEST_CASE("vtable",
          "[vtable]") {

  using tite::detail::vtable_for;
  using tite::overload;

  using times_two_ovld = overload<times_two, int(tite::self)>;

  auto const& vt = vtable_for<int, times_two_ovld>;

  int i = 5;
  void* vptr = &i;
  CHECK( vt.template fn<times_two_ovld>()(times_two_ovld{}, vptr) == 10 );

# ifdef TITE_ENABLE_CUDA
  CHECK( run_vtable_cuda_test() == 10 );
# endif
}
