#ifndef TITE_VTABLE_HPP
#define TITE_VTABLE_HPP

#include "tite/self.hpp"

#include <type_traits>
#include <utility>

namespace tite {

namespace detail {

template<typename T,
         typename CPOT,
         typename ReturnT,
         typename... ArgsT>
constexpr auto invoke(CPOT cpo,
                      replace_self_with_void_ptr_t<ArgsT>... args)
noexcept( noexcept( std::move(cpo)(replace_self<ArgsT>::get((ArgsT &&) args, std::declval<T&>())...) ) )
-> ReturnT
{
  void* self_ptr = extract_self<ArgsT...>{}(args...);
  T& obj = *static_cast<T*>(self_ptr);
  return std::move(cpo)(replace_self<ArgsT>::get((ArgsT &&) args, obj)...);
}

template<typename T,
         typename CPOT,
         typename ReturnT,
         typename... ArgsT>
struct default_policy {
  template<typename FunctionT>
  constexpr void operator()(FunctionT*& ptr) const
  noexcept
  {
    ptr = invoke<T,CPOT,ReturnT,ArgsT...>;
  }
};

template<typename CPOT,
         typename Sig> // = typename CPOT::erased_signature_t>
struct vtable_entry;

template<typename CPOT,
         typename ReturnT,
         typename... ArgsT>
struct vtable_entry<CPOT, ReturnT(ArgsT...)> {
private:
  using function_t = ReturnT(CPOT, replace_self_with_void_ptr_t<ArgsT>...);

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
      return invoke<T,CPOT,ReturnT,ArgsT...>;
    }
  };

  template<typename T,
           template<typename...> class PointerPolicy=default_policy>
  static constexpr vtable_entry construct() noexcept {
    function_t* ptr = nullptr;
    PointerPolicy<T,CPOT,ReturnT,ArgsT...>{}(ptr);
    return vtable_entry{ ptr };
  }
};

} // namespace detail

} // namespace tite

#endif
