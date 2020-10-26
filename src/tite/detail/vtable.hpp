#ifndef TITE_DETAIL_VTABLE_HPP
#define TITE_DETAIL_VTABLE_HPP

#include "tite/detail/vtable_entry.hpp"

namespace tite {

namespace detail {

template<typename... OverloadsT>
struct vtable : private vtable_entry<OverloadsT>... {
public:
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
    vtable_entry<OverloadT> const& vtable_entry_base = *this;
    return vtable_entry_base.fn();
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
