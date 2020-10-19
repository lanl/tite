#ifndef TITE_SELF_HPP
#define TITE_SELF_HPP

#include "tite/type_traits/requires.hpp"
#include "tite/type_traits/inline_var.hpp"

#include <type_traits>

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

template<typename ArgT, typename T>
using replace_self_with_void_ptr_t =
  std::conditional_t<
    std::is_same<std::remove_cv_t<std::remove_reference_t<ArgT>>, self>::value,
    void*,
    ArgT>;

struct extract_self_fn {
private:
  struct impl {
    template<typename FirstT,
             typename... RestT,
             TITE_REQUIRES( std::is_same<std::remove_cv_t<std::remove_reference_t<FirstT>>, self>::value )>
    constexpr auto operator()(FirstT&& f, RestT&&... r) const
    noexcept
    -> decltype( f )
    {
      return f;
    }

    template<typename FirstT,
             typename... RestT,
             TITE_REQUIRES( not std::is_same<std::remove_cv_t<std::remove_reference_t<FirstT>>, self>::value ),
             TITE_REQUIRES( (sizeof...(RestT) > 0) )>
    constexpr auto operator()(FirstT&& f, RestT&&... r) const
    noexcept
    -> decltype( std::declval<impl const&>()( std::forward<RestT>(r)... ) )
    {
      return (*this)(std::forward<RestT>(r)...);
    }
  };

public:
  template<typename... ArgsT>
  constexpr auto operator()(ArgsT&&... args) const
  noexcept
  -> decltype( impl{}(std::forward<ArgsT>(args)...) )
  {
    return impl{}(std::forward<ArgsT>(args)...);
  }
};

TITE_INLINEVAR(extract_self_fn const, extract_self)

} // namespace tite

#endif
