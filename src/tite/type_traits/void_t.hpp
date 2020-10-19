#ifndef TITE_TYPE_TRAITS_VOID_T_HPP
#define TITE_TYPE_TRAITS_VOID_T_HPP

/* see https://en.cppreference.com/w/cpp/types/void_t */

namespace tite {

inline namespace type_traits {

template <typename...>
using void_t = void;

} // inline namespace type_traits

} // namespace tite

#endif
