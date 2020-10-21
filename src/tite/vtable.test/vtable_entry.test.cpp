#include "tite.hpp"
#include "times_two.hpp"

#include "catch2/catch.hpp"

#ifdef TITE_ENABLE_CUDA
int run_vtable_entry_cuda_test();
#endif

TEST_CASE("vtable_entry",
          "[vtable_entry]") {

  using tite::detail::vtable_entry;
  using tite::overload;

  using times_two_ovld = overload<times_two, int(tite::self)>;

  auto vte = vtable_entry<times_two_ovld>::construct<int>();

  int i = 5;
  void* vptr = &i;
  CHECK( vte.fn()(times_two{}, vptr) == 10 );

# ifdef TITE_ENABLE_CUDA
  CHECK( run_vtable_entry_cuda_test() == 10 );
# endif
}
