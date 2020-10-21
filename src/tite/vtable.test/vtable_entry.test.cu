#include "tite.hpp"
#include "tite/vtable/cuda_invoke_pointer_policy.hpp"
#include "cuda.h"

#include <cstdio>

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
  noexcept( noexcept( tite::tag_invoke{}(std::declval<times_two_cpo const&>(), std::forward<ArgT>(arg)) ) )
  -> decltype( tite::tag_invoke{}(std::declval<times_two_cpo const&>(), std::forward<ArgT>(arg)) )
  {
    return tite::tag_invoke{}(*this, std::forward<ArgT>(arg));
  }
};

template<typename VTableEntryT>
__global__ void call(VTableEntryT vte)
{
  int i=5;
  void* vptr = &i;
  printf("result = %i\n", vte.fn()(times_two_cpo{}, vptr));
}

template<typename T>
constexpr void foo() {}; //{printf("%s\n", __PRETTY_FUNCTION__);}

int main() {
  using tite::detail::vtable_entry;

  auto vte = vtable_entry<times_two_cpo, int(tite::self)>::construct<int,tite::cuda_invoke_pointer_policy>();

  call<<<1,1>>>(vte);
  cudaDeviceSynchronize();
}
