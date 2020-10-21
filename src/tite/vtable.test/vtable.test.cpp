#include "tite.hpp"

#include "catch2/catch.hpp"

struct times_two {
private:
  template<typename ArgT>
  friend constexpr auto tag_invoke(times_two, ArgT&& arg)
  noexcept
  -> decltype( 2*std::forward<ArgT>(arg) )
  {
    return 2 * std::forward<ArgT>(arg);
  }

public:
  template<typename ArgT>
  constexpr auto operator()(ArgT&& arg) const
  noexcept( noexcept( tite::tag_invoke{}(std::declval<times_two const&>(), std::forward<ArgT>(arg)) ) )
  -> decltype( tite::tag_invoke{}(std::declval<times_two const&>(), std::forward<ArgT>(arg)) )
  {
    return tite::tag_invoke{}(*this, std::forward<ArgT>(arg));
  }
};

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
