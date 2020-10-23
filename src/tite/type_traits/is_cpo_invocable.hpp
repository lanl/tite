#ifndef TITE_TYPE_TRAITS_IS_CPO_INVOCABLE_HPP
#define TITE_TYPE_TRAITS_IS_CPO_INVOCABLE_HPP

#include "tite/self.hpp"
#include "tite/type_traits/requires.hpp"
#include "tite/type_traits/invocable.hpp"

#include <type_traits>

namespace tite {

template<typename T,
         typename CPOT,
         typename SignatureT = typename CPOT::signature_t>
struct is_cpo_invocable : std::false_type {};

template<typename T,
         typename CPOT,
         typename ReturnT,
         typename... ArgsT>
struct is_cpo_invocable<T, CPOT, ReturnT(ArgsT...)> {
private:
  template<typename T_, bool>
  struct impl : std::false_type {};

  template<typename T_>
  struct impl<T_, true> {
    constexpr static bool value = std::is_convertible<invoke_result_t<CPOT, tite::detail::replace_self_t<ArgsT,T_>...>,
                                                      ReturnT>::value;
  };

public:
  constexpr static bool value = impl<T, is_invocable_v<CPOT, tite::detail::replace_self_t<ArgsT,T>...>>::value;
};

template<typename T,
         typename CPOT>
static constexpr bool is_cpo_invocable_v = is_cpo_invocable<T,CPOT>::value;

} // namespace tite

#endif
