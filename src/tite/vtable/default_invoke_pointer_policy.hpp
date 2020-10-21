#ifndef TITE_VTABLE_DEFAULT_INVOKE_POINTER_POLICY_HPP
#define TITE_VTABLE_DEFAULT_INVOKE_POINTER_POLICY_HPP

#include "tite/vtable/invoke.hpp"

namespace tite {

template<typename T,
         typename CPOT,
         typename ReturnT,
         typename... ArgsT>
struct default_invoke_pointer_policy {
  template<typename FunctionT>
  constexpr void operator()(FunctionT*& ptr) const
  noexcept
  {
    ptr = invoke<T,CPOT,ReturnT,ArgsT...>;
  }
};

} // namespace tite

#endif
