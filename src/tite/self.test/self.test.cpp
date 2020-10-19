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
    self s1;
    CHECK( std::is_same<decltype(tite::extract_self(5.0, s1, 1)), self&>::value );

    self const s2;
    CHECK( std::is_same<decltype(tite::extract_self(5.0, 1, s2)), self const&>::value );

    self const&& s3 = std::move(s1);
    CHECK( std::is_same<decltype(tite::extract_self(std::move(s3) ,5.0, 1)), self const&&>::value );

    CHECK( std::is_same<decltype(tite::extract_self(self{},5.0, 1)), self&&>::value );
  }
}
