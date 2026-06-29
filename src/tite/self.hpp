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
#ifndef TITE_SELF_HPP
#define TITE_SELF_HPP

#include "tite/type_traits/requires.hpp"

#include <type_traits>
#include <utility>

namespace tite {

struct self {};

namespace detail {

template<typename NotSelfT>
struct replace_self {
  template<typename ArgT, typename T>
  using type = ArgT;

  template<typename ArgT, typename T>
  constexpr static ArgT&& get(ArgT&& arg, T& /*obj*/)
  noexcept
  {
    return std::forward<ArgT>(arg);
  }
};

template<>
struct replace_self<self> {
  template<typename ArgT, typename T>
  using type = T;

  template<typename ArgT, typename T>
  constexpr static T&& get(ArgT&& /*arg*/, T& obj)
  noexcept
  {
    return static_cast<T &&>(obj);
  }
};

template<>
struct replace_self<self&> {
  template<typename ArgT, typename T>
  using type = T&;

  template<typename ArgT, typename T>
  constexpr static T& get(ArgT&& /*arg*/, T& obj)
  noexcept
  {
    return obj;
  }
};

template<>
struct replace_self<self&&> {
  template<typename ArgT, typename T>
  using type = T&&;

  template<typename ArgT, typename T>
  constexpr static T&& get(ArgT&& /*arg*/, T& obj)
  noexcept
  {
    return static_cast<T &&>(obj);
  }
};

template<>
struct replace_self<self const&> {
  template<typename ArgT, typename T>
  using type = T const&;

  template<typename ArgT, typename T>
  constexpr static T const&  get(ArgT&& /*arg*/, T& obj)
  noexcept
  {
    return obj;
  }
};

template<>
struct replace_self<self const&&> {
  template<typename ArgT, typename T>
  using type = T const&&;

  template<typename ArgT, typename T>
  constexpr static auto get(ArgT&& /*arg*/, T& obj)
  noexcept( noexcept( static_cast<T const&&>(obj) ) )
  -> decltype( static_cast<T const&&>(obj) )
  {
    return static_cast<T const&&>(obj);
  }
};

template<typename ArgT, typename T>
using replace_self_t = typename replace_self<ArgT>::template type<ArgT,T>;

template<typename ArgT>
using replace_self_with_void_ptr_t =
  std::conditional_t<
    std::is_same<std::remove_cv_t<std::remove_reference_t<ArgT>>, self>::value,
    void*,
    ArgT>;

template<typename FirstT, typename... RestT>
struct extract_self {
  template<typename FirstT_,
           typename... RestT_,
           typename MaybeSelf = FirstT,
           TITE_REQUIRES(std::is_same<std::remove_cv_t<std::remove_reference_t<MaybeSelf>>, self>::value)>
  constexpr auto operator()(FirstT_&& f, RestT_&&... rest) const
  noexcept
  -> decltype( std::forward<FirstT_>(f) )
  {
    return std::forward<FirstT_>(f);
  }

  template<typename FirstT_,
           typename... RestT_,
           typename MaybeSelf = FirstT,
           TITE_REQUIRES(not std::is_same<std::remove_cv_t<std::remove_reference_t<MaybeSelf>>, self>::value)>
  constexpr auto operator()(FirstT_&& f, RestT_&&... rest) const
  noexcept
  -> decltype( extract_self<RestT...>{}(std::forward<RestT_>(rest)...) )
  {
    return extract_self<RestT...>{}(std::forward<RestT_>(rest)...);
  }
};

template<typename OnlyT>
struct extract_self<OnlyT> {
  template<typename OnlyT_,
           typename MaybeSelf = OnlyT,
           TITE_REQUIRES(std::is_same<std::remove_cv_t<std::remove_reference_t<MaybeSelf>>, self>::value)>
  constexpr auto operator()(OnlyT_&& f) const
  noexcept
  -> decltype( std::forward<OnlyT_>(f) )
  {
    return std::forward<OnlyT_>(f);
  }
};

} // namespace detail

} // namespace tite

#endif
