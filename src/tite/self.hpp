#ifndef TITE_SELF_HPP
#define TITE_SELF_HPP

#include "tite/type_traits/requires.hpp"
#include "tite/type_traits/inline_var.hpp"

#include <type_traits>
#include <utility>

namespace tite {

struct self {};

template<typename NotSelfT>
struct replace_self {
  template<typename ArgT, typename T>
  using type = ArgT;

  template<typename ArgT, typename T>
  constexpr static auto get(ArgT&& arg, T& /*obj*/)
  noexcept( noexcept( static_cast<ArgT &&>(arg) ) )
  -> decltype( static_cast<ArgT &&>(arg) )
  {
    return static_cast<ArgT &&>(arg);
  }
};

template<>
struct replace_self<self> {
  template<typename ArgT, typename T>
  using type = T;

  template<typename ArgT, typename T>
  constexpr static auto get(ArgT&& /*arg*/, T& obj)
  noexcept( noexcept( static_cast<T &&>(obj) ) )
  -> decltype( static_cast<T &&>(obj) )
  {
    return static_cast<T &&>(obj);
  }
};

template<>
struct replace_self<self&> {
  template<typename ArgT, typename T>
  using type = T&;

  template<typename ArgT, typename T>
  constexpr static auto get(ArgT&& /*arg*/, T& obj)
  noexcept
  -> T&
  {
    return obj;
  }
};

template<>
struct replace_self<self&&> {
  template<typename ArgT, typename T>
  using type = T&&;

  template<typename ArgT, typename T>
  constexpr static auto get(ArgT&& /*arg*/, T& obj)
  noexcept( noexcept( static_cast<T &&>(obj) ) )
  -> decltype( static_cast<T &&>(obj) )
  {
    return static_cast<T &&>(obj);
  }
};

template<>
struct replace_self<self const&> {
  template<typename ArgT, typename T>
  using type = T const&;

  template<typename ArgT, typename T>
  constexpr static auto get(ArgT&& /*arg*/, T& obj)
  noexcept
  -> T const&
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

} // namespace tite

#endif
