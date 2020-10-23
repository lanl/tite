#ifndef TITE_TYPE_TRAITS_IS_TYPE_ERASABLE_HPP
#define TITE_TYPE_TRAITS_IS_TYPE_ERASABLE_HPP

#include "tite/type_traits/all_of.hpp"
#include "tite/type_traits/is_cpo_invocable.hpp"

namespace tite {

template<typename T, typename... OverloadsT>
struct is_type_erasable {
private:
  template<typename OverloadT>
  using this_one_is_type_erasable = is_cpo_invocable<T,OverloadT>;

public:
  static constexpr bool value = all_of_v<this_one_is_type_erasable, OverloadsT...>;
};

template<typename T, typename... OverloadsT>
static constexpr bool is_type_erasable_v = is_type_erasable<T, OverloadsT...>::value;

}

#endif
