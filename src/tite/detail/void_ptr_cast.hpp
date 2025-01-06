#ifndef TITE_DETAIL_VOID_PTR_CAST_HPP
#define TITE_DETAIL_VOID_PTR_CAST_HPP

#include "tite/type_traits/requires.hpp"
#include "tite/type_traits/tag_invoke.hpp"

namespace tite {

template<typename CastT>
struct void_ptr_cast_cpo {

  template<typename TagT>
  constexpr
  friend auto tag_invoke(void_ptr_cast_cpo, void* ptr, TagT)
  noexcept
  -> CastT&
  {
    return *static_cast<CastT*>(ptr);
  }

  template<typename TagT,
           TITE_REQUIRES(is_tag_invocable<void_ptr_cast_cpo, void*, TagT>::value)>
  constexpr auto operator()(void* ptr, TagT tag) const
  noexcept
  -> CastT&
  {
    return tag_invoke_fn{}(void_ptr_cast_cpo{}, ptr, tag);
  }
};

template<typename CastT>
constexpr static auto void_ptr_cast = void_ptr_cast_cpo<CastT>{};

} // namespace tite

#endif
