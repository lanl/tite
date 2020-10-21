#ifndef TITE_VTABLE_HPP
#define TITE_VTABLE_HPP

#include "tite/self.hpp"
#include "tite/vtable/default_invoke_pointer_policy.hpp"

#include <type_traits>
#include <utility>

namespace tite {

namespace detail {

template<typename OverloadT,
         typename SignatureT = typename OverloadT::signature_t>
struct vtable_entry;

template<typename OverloadT,
         typename ReturnT,
         typename... ArgsT>
struct vtable_entry<OverloadT, ReturnT(ArgsT...)> {
protected:
  using function_t = ReturnT(OverloadT, replace_self_with_void_ptr_t<ArgsT>...);

  explicit constexpr
  vtable_entry(function_t* fn)
  noexcept
  : fn_{ fn }
  {}

  function_t* fn_;

public:
  constexpr function_t* fn() const noexcept {
    return *fn_;
  }

  template<typename T>
  struct obtainer {
    constexpr function_t* operator()() const
    noexcept
    {
      return invoke<T,OverloadT,ReturnT,ArgsT...>;
    }
  };

  template<typename T,
           template<typename...> class InvokePointerPolicy=default_invoke_pointer_policy>
  static constexpr vtable_entry construct() noexcept {
    function_t* ptr = nullptr;
    InvokePointerPolicy<T,OverloadT,ReturnT,ArgsT...>{}(ptr);
    return vtable_entry{ ptr };
  }
};


template<typename... OverloadsT>
struct vtable : private vtable_entry<OverloadsT>... {
  template<typename T,
           template<typename...> class InvokePointerPolicy=default_invoke_pointer_policy>
  static constexpr vtable construct()
  noexcept
  {
    return vtable{vtable_entry<OverloadsT>::template construct<T,InvokePointerPolicy>()...};
  }

  template<typename OverloadT>
  constexpr auto fn() const
  noexcept
  -> typename vtable_entry<OverloadT>::function_t*
  {
    return vtable_entry<OverloadT>::fn();
  }

private:
  explicit constexpr
  vtable(vtable_entry<OverloadsT>... entries)
  : vtable_entry<OverloadsT>{entries}...
  {}
};

} // namespace detail

} // namespace tite

#endif
