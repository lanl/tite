#include "tite/tite.hpp"

#include "get_cpo.hpp"

#include "catch2/catch_test_macros.hpp"

#include <vector>
#include <deque>

#ifdef TITE_ENABLE_GPU
int run_any_unique_gpu_test();
#endif

TEST_CASE("any_unique",
          "[any_unique]") {

  using tite::any_unique;
  using tite::overload;
  using tite::self;

  SECTION("mutable any_unique requires deep_copy_base") {
    any_unique< overload<get_cpo, int&(self&, int)>,
                overload<get_cpo, int const&(self const&, int)> >
                const au1 = {std::deque<int>{1,2,3,4,5}, std::allocator<unsigned char>{}};

    decltype(au1) au2{au1};
    CHECK(au2.get_object_address() != au1.get_object_address());

    tite::remove_cvref_t<decltype(au1)> au3{au1};

    au3 = au2;
    CHECK(au3.get_object_address() != au2.get_object_address());

    get(au3,0) = 5;

    CHECK( get(au1, 2) == 3 );
    CHECK( get(au2, 4) == 5 );
    CHECK( get(au3, 0) == 5 );
  }

  SECTION("immutable any_unique uses reference_counted_copy_base") {
    any_unique< overload<get_cpo, int const&(self const&, int)> >
                const au1 = {std::vector<int>{1,2,3,4,5}, std::allocator<unsigned char>{}};

    decltype(au1) au2{au1};
    CHECK(au2.get_object_address() == au1.get_object_address());

    tite::remove_cvref_t<decltype(au1)> au3{au1};

    au3 = au2;
    CHECK(au3.get_object_address() == au2.get_object_address());

    CHECK( get(au1, 2) == 3 );
    CHECK( get(au2, 4) == 5 );
    CHECK( get(au3, 0) == 1 );
  }

  SECTION("mutable any_unique forced to referenced_counted_copy_base") {
    any_unique< overload<get_cpo, int&(self&, int)>,
                overload<get_cpo, int const&(self const&, int)> >
                const au1 = {std::deque<int>{1,2,3,4,5}, std::allocator<unsigned char>{},
                tite::copy_reference_counted};

    decltype(au1) au2{au1};
    CHECK(au2.get_object_address() == au1.get_object_address());

    tite::remove_cvref_t<decltype(au1)> au3{au1};

    au3 = au2;
    CHECK(au3.get_object_address() == au2.get_object_address());

    get(au3,0) = 5;

    CHECK( get(au1, 2) == 3 );
    CHECK( get(au2, 4) == 5 );
    CHECK( get(au3, 0) == 5 );
  }

  SECTION("immutable any_unique forced to deep_copy_base") {
    any_unique< overload<get_cpo, int const&(self const&, int)> >
                const au1 = {std::vector<int>{1,2,3,4,5}, std::allocator<unsigned char>{},
                tite::copy_deep};

    decltype(au1) au2{au1};
    CHECK(au2.get_object_address() != au1.get_object_address());

    tite::remove_cvref_t<decltype(au1)> au3{au1};

    au3 = au2;
    CHECK(au3.get_object_address() != au2.get_object_address());

    CHECK( get(au1, 2) == 3 );
    CHECK( get(au2, 4) == 5 );
    CHECK( get(au3, 0) == 1 );
  }

#ifdef TITE_ENABLE_GPU
  run_any_unique_gpu_test();
#endif
}
