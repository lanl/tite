#include "tite.hpp"

#include "catch2/catch.hpp"

struct times_two_cpo {
private:
  template<typename ArgT>
  friend constexpr auto tag_invoke(times_two_cpo, ArgT&& arg)
  noexcept
  -> decltype( 2*std::forward<ArgT>(arg) )
  {
    return 2 * std::forward<ArgT>(arg);
  }

public:
  template<typename ArgT>
  constexpr auto operator()(ArgT&& arg) const
  noexcept( noexcept( tite::tag_invoke(std::declval<times_two_cpo const&>(), std::forward<ArgT>(arg)) ) )
  -> decltype( tite::tag_invoke(std::declval<times_two_cpo const&>(), std::forward<ArgT>(arg)) )
  {
    return tite::tag_invoke(*this, std::forward<ArgT>(arg));
  }
};

static constexpr times_two_cpo times_two{};

TEST_CASE("vtable_entry",
          "[vtable_entry]") {

  using tite::detail::vtable_entry;

  auto vte = vtable_entry<times_two_cpo, int(tite::self)>::construct<int>();

  int i = 5;
  void* vptr = &i;
  CHECK( vte.fn()(times_two, vptr) == 10 );
}
