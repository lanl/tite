#include "tite.hpp"

#include "catch2/catch.hpp"
#include "tite/self.hpp"

TEST_CASE("self",
          "[self]") {

  using tite::self;

  SECTION("test replace_self_t with non-self type") {
    CHECK( std::is_same<tite::replace_self_t<double,int>, double>::value );
    CHECK( std::is_same<tite::replace_self_t<double&,int>, double&>::value );
    CHECK( std::is_same<tite::replace_self_t<double const&,int>, double const&>::value );
    CHECK( std::is_same<tite::replace_self_t<double&&,int>, double&&>::value );
    CHECK( std::is_same<tite::replace_self_t<double const&&,int>, double const&&>::value );
  }

  SECTION("test replace_self_t with self type") {
    CHECK( std::is_same<tite::replace_self_t<self,int>, int>::value );
    CHECK( std::is_same<tite::replace_self_t<self&,int>, int&>::value );
    CHECK( std::is_same<tite::replace_self_t<self const&,int>, int const&>::value );
    CHECK( std::is_same<tite::replace_self_t<self&&,int>, int&&>::value );
    CHECK( std::is_same<tite::replace_self_t<self const&&,int>, int const&&>::value );
  }

  SECTION("test extract_self") {
    CHECK( tite::extract_self<self>{}('c') == 'c' );

    CHECK( tite::extract_self<double, self>{}(5.0, 'c') == 'c' );
    CHECK( tite::extract_self<self, char>{}(5.0, 'c') == 5.0 );

    CHECK( tite::extract_self<double, int, self>{}(5.0, 1, 'c') == 'c' );
    CHECK( tite::extract_self<double, self, char>{}(5.0, 1, 'c') == 1 );
    CHECK( tite::extract_self<self, int, char>{}(5.0, 1, 'c') == 5.0 );
  }
}
