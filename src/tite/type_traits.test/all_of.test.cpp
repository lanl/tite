#include "tite.hpp"

#include "catch2/catch.hpp"

template<typename T>
using is_int = std::is_same<T,int>;

TEST_CASE("all_of",
          "[type_traits][all_of]") {

  using tite::all_of_v;

  CHECK( all_of_v<is_int, int, int, int> );
  CHECK( not all_of_v<is_int, int, int, double> );
  CHECK( not all_of_v<is_int, int, char, int> );
  CHECK( not all_of_v<is_int, bool, int, int> );
}
