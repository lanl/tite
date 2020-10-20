#ifndef TITE_VTABLE_HPP
#define TITE_VTABLE_HPP

#include "tite/self.hpp"
#include <type_traits>

namespace tite {

namespace detail {

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

  template<typename T>
  static constexpr auto invoke(CPOT cpo,
                               replace_self_with_void_ptr_t<ArgsT>... args)
  noexcept( noexcept( std::move(cpo)(replace_self<ArgsT>::get((ArgsT &&) args, std::declval<T&>())...) ) )
  -> ReturnT
  {
    void* self_ptr = extract_self<ArgsT...>{}(args...);
    T& obj = *static_cast<T*>(self_ptr);
    return std::move(cpo)(replace_self<ArgsT>::get((ArgsT &&) args, obj)...);
  }

  template<typename T>
  struct obtainer {
    constexpr function_t* operator()() const
    noexcept
    {
      return invoke<T>;
    }
  };

  struct default_obtain {
    template<typename FunctionT, typename ObtainerFn>
    constexpr void operator()(FunctionT*& ptr, ObtainerFn&& obtainer) const
    noexcept( noexcept( ptr = std::forward<ObtainerFn>(obtainer)() ) )
    {
      ptr = std::forward<ObtainerFn>(obtainer)();
    }
  };

public:
  constexpr function_t* fn() const noexcept {
    return fn_;
  }

  template<typename T,
           typename ObtainFn=default_obtain>
  static constexpr vtable_entry construct(ObtainFn obtain={}) noexcept {
    function_t* ptr = nullptr;
    obtain(ptr, obtainer<T>{});
    return vtable_entry{ ptr };
  }
};

} // namespace detail

} // namespace tite

#endif
