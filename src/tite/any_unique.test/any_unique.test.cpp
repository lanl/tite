#include "tite.hpp"

#include "get_cpo.hpp"

#include "catch2/catch_test_macros.hpp"

#include <type_traits>

#include <deque>

#ifdef TITE_ENABLE_GPU
int run_any_unique_gpu_test();
#endif

TEST_CASE("any_unique",
          "[any_unique]") {

  using tite::any_unique;
  using tite::overload;
  using tite::self;

  any_unique< overload<get_cpo, int&(self&, int)>,
              overload<get_cpo, int const&(self const&, int)> > const au1 = {std::deque<int>{1,2,3,4,5}, std::allocator<unsigned char>{}};

  // decltype(au1) au2{au1};

  // tite::remove_cvref_t<decltype(au1)> au3{au1};

  // au3 = au2;

  // get(au3,0) = 5;

  CHECK( get(au1, 2) == 3 );
  // CHECK( get(au2, 4) == 5 );
  // CHECK( get(au3, 0) == 5 );

#ifdef TITE_ENABLE_GPU
  run_any_unique_gpu_test();
#endif
}
