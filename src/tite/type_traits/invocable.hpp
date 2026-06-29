/*
 * © 2025. Triad National Security, LLC. All rights reserved.
 * This program was produced under U.S. Government contract 89233218CNA000001
 * for Los Alamos National Laboratory (LANL), which is operated by Triad
 * National Security, LLC for the U.S. Department of Energy/National Nuclear
 * Security Administration. All rights in the program are reserved by Triad
 * National Security, LLC, and the U.S. Department of Energy/National Nuclear
 * Security Administration. The Government is granted for itself and others
 * acting on its behalf a nonexclusive, paid-up, irrevocable worldwide license
 * in this material to reproduce, prepare derivative works, distribute copies
 * to the public, perform publicly and display publicly, and to permit others
 * to do so.
 */
#ifndef TITE_TYPE_TRAITS_INVOCABLE_HPP
#define TITE_TYPE_TRAITS_INVOCABLE_HPP

#include "tite/type_traits/void_t.hpp"
#include "tite/type_traits/is_specialization_of.hpp"
#include "tite/type_traits/requires.hpp"

#include <type_traits>
#include <utility>

namespace tite {

namespace detail {

template<class T, class Type, class T1, class... Args,
         TITE_REQUIRES( std::is_member_function_pointer<Type T::*>::value
                    and not std::is_base_of<T, std::decay_t<T1>>::value )>
auto INVOKE(Type T::* f, T1&& t1, Args&&... args)
  noexcept(noexcept((std::forward<T1>(t1).*f)(std::forward<Args>(args)...)))
  -> decltype((std::forward<T1>(t1).*f)(std::forward<Args>(args)...)) {
  return (std::forward<T1>(t1).*f)(std::forward<Args>(args)...);
}

template<class T, class Type, class T1, class... Args,
         TITE_REQUIRES( std::is_member_function_pointer<Type T::*>::value
                    and not std::is_base_of<T, std::decay_t<T1>>::value
                    and is_specialization_of_v<std::reference_wrapper, std::decay_t<T1>> )>
auto INVOKE(Type T::* f, T1&& t1, Args&&... args)
  noexcept(noexcept((t1.get().*f)(std::forward<Args>(args)...)))
  -> decltype((t1.get().*f)(std::forward<Args>(args)...)) {
  return (t1.get().*f)(std::forward<Args>(args)...);
}

template<class T, class Type, class T1, class... Args,
         TITE_REQUIRES( std::is_member_function_pointer<Type T::*>::value
                    and not std::is_base_of<T, std::decay_t<T1>>::value
                    and not is_specialization_of_v<std::reference_wrapper, std::decay_t<T1>> )>
auto INVOKE(Type T::* f, T1&& t1, Args&&... args)
  noexcept(noexcept(((*std::forward<T1>(t1)).*f)(std::forward<Args>(args)...)))
  -> decltype(((*std::forward<T1>(t1)).*f)(std::forward<Args>(args)...)) {
  return ((*std::forward<T1>(t1)).*f)(std::forward<Args>(args)...);
}

template<class T, class Type, class T1,
         TITE_REQUIRES( std::is_member_object_pointer<Type T::*>::value
                    and std::is_base_of<T, std::decay_t<T1>>::value )>
auto INVOKE(Type T::* f, T1&& t1)
  noexcept(noexcept(std::forward<T1>(t1).*f))
  -> decltype(std::forward<T1>(t1).*f) {
  return std::forward<T1>(t1).*f;
}

template<class T, class Type, class T1,
         TITE_REQUIRES( std::is_member_object_pointer<Type T::*>::value
                    and not std::is_base_of<T, std::decay_t<T1>>::value
                    and is_specialization_of_v<std::reference_wrapper, std::decay_t<T1>> )>
auto INVOKE(Type T::* f, T1&& t1)
  noexcept(noexcept(t1.get().*f))
  -> decltype(t1.get().*f) {
  return t1.get().*f;
}

template<class T, class Type, class T1,
         TITE_REQUIRES( std::is_member_object_pointer<Type T::*>::value
                    and not std::is_base_of<T, std::decay_t<T1>>::value
                    and not is_specialization_of_v<std::reference_wrapper, std::decay_t<T1>> )>
auto INVOKE(Type T::* f, T1&& t1)
  noexcept(noexcept((*std::forward<T1>(t1)).*f))
  -> decltype((*std::forward<T1>(t1)).*f) {
  return (*std::forward<T1>(t1)).*f;
}

template <class F, class... Args,
          TITE_REQUIRES( not std::is_member_object_pointer<F>::value
                     and not std::is_member_function_pointer<F>::value )>
auto INVOKE(F&& f, Args&&... args)
  noexcept(noexcept(std::forward<F>(f)(std::forward<Args>(args)...)))
  -> decltype(std::forward<F>(f)(std::forward<Args>(args)...)) {
  return std::forward<F>(f)(std::forward<Args>(args)...);
}

} // namespace detail

template<typename... ArgsT>
struct invoke_result {
  using type = decltype(detail::INVOKE(std::declval<ArgsT>()...));
};

template<typename... ArgsT>
using invoke_result_t = typename invoke_result<ArgsT...>::type;

template<typename T, typename... ArgsT>
struct is_invocable {
private:
  template<typename T_, typename = void>
  struct impl : std::false_type {};

  template<typename T_>
  struct impl<T_, void_t<decltype(detail::INVOKE(std::declval<T_>(), std::declval<ArgsT>()...))>> : std::true_type {};

public:
  constexpr static bool value = impl<T>::value;
};

template<typename... ArgsT>
constexpr static bool is_invocable_v = is_invocable<ArgsT...>::value;

template<typename T, typename... ArgsT>
struct is_nothrow_invocable {
private:
  template<typename T_=T, typename = void>
  struct impl : std::false_type {};

  template<typename T_>
  struct impl<T_, void_t<decltype(detail::INVOKE(std::declval<T_>(), std::declval<ArgsT>()...))>> {
    constexpr static bool value = noexcept( detail::INVOKE(std::declval<T_>(), std::declval<ArgsT>()...) );
  };

public:
  constexpr static bool value = impl<T>::value;
};

template<typename... ArgsT>
constexpr static bool is_nothrow_invocable_v = is_nothrow_invocable<ArgsT...>::value;

} // namespace tite

#endif
