#ifndef TITE_DETAIL_VTABLE_ENTRY_HPP
#define TITE_DETAIL_VTABLE_ENTRY_HPP

#include "tite/self.hpp"
#include "tite/default_invoke_pointer_policy.hpp"

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

  template<typename T,
           template<typename...> class InvokePointerPolicy=default_invoke_pointer_policy>
  static constexpr vtable_entry construct() noexcept {
    function_t* ptr = nullptr;
    InvokePointerPolicy<T,OverloadT,ReturnT,ArgsT...>{}(ptr);
    return vtable_entry{ ptr };
  }
};

} // namespace detail

} // namespace tite

#endif
