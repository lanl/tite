#ifndef TITE_DETAIL_VTABLE_ENTRY_HPP
#define TITE_DETAIL_VTABLE_ENTRY_HPP

#include "tite/self.hpp"
#include "tite/detail/vtable_invoke.hpp"

namespace tite {

namespace detail {

template<typename OverloadT,
         typename SignatureT = typename OverloadT::signature_t>
struct vtable_entry;

template<typename OverloadT,
         typename ReturnT,
         typename... ArgsT>
struct vtable_entry<OverloadT, ReturnT(ArgsT...)> {
public:
  using function_t = ReturnT(OverloadT, replace_self_with_void_ptr_t<ArgsT>...);

protected:
  constexpr
  vtable_entry()
  noexcept
  : fn_{ nullptr }
  {}

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
  static constexpr vtable_entry construct() noexcept {
    return vtable_entry{ &vtable_invoke<T,OverloadT,ReturnT,ArgsT...> };
  }
};

} // namespace detail

} // namespace tite

#endif
