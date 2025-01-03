#ifndef TITE_DETAIL_VTABLE_HPP
#define TITE_DETAIL_VTABLE_HPP

#include "tite/detail/vtable_entry.hpp"
#include "tite/type_traits/requires.hpp"
#include "tite/type_traits/tag_invoke.hpp"

#include <memory>

namespace tite {

namespace detail {

template<typename... OverloadsT>
struct vtable : private vtable_entry<OverloadsT>... {
public:
  template<typename T>
  static constexpr vtable construct()
  noexcept
  {
    return vtable{vtable_entry<OverloadsT>::template construct<T>()...};
  }

  template<typename OverloadT>
  constexpr auto fn() const
  noexcept
  -> typename vtable_entry<OverloadT>::function_t*
  {
    vtable_entry<OverloadT> const& vtable_entry_base = *this;
    return vtable_entry_base.fn();
  }

  constexpr
  vtable() = default;
private:
  explicit constexpr
  vtable(vtable_entry<OverloadsT>... entries)
  : vtable_entry<OverloadsT>{entries}...
  {}
};

template<typename T, typename... OverloadsT>
constexpr static vtable<OverloadsT...> vtable_for = vtable<OverloadsT...>::template construct<T>();

template<typename T, typename... OverloadsT>
struct default_vtable_pointer_policy {
  constexpr vtable<OverloadsT...> const*
  operator()()
  noexcept
  {
    return &vtable_for<T,OverloadsT...>;
  }

  constexpr void
  operator()(vtable<OverloadsT...>& v)
  noexcept
  {
    v = vtable_for<T,OverloadsT...>;
  }
};

} // namespace detail

template<typename T, typename... OverloadsT>
struct get_vtable_pointer_cpo {

  friend constexpr auto tag_invoke(get_vtable_pointer_cpo, std::allocator<T>)
  noexcept
  -> decltype( std::addressof(detail::vtable_for<T, OverloadsT...>) )
  {
    return std::addressof(detail::vtable_for<T, OverloadsT...>);
  }

  template<typename AllocT,
           TITE_REQUIRES(is_tag_invocable_v<get_vtable_pointer_cpo, AllocT&&>)>
  constexpr auto operator()(AllocT&& t) const
  noexcept( noexcept( tag_invoke_fn{}(std::declval<get_vtable_pointer_cpo>(), std::forward<AllocT>(t)) ) )
  -> decltype( tag_invoke_fn{}(std::declval<get_vtable_pointer_cpo>(), std::forward<AllocT>(t)) )
  {
    return tag_invoke_fn{}(*this, std::forward<AllocT>(t));
  }
};

template<typename... OverloadsT>
constexpr static auto get_vtable_pointer = get_vtable_pointer_cpo<OverloadsT...>{};

template<typename T, typename... OverloadsT>
struct get_vtable_cpo {

  friend constexpr auto tag_invoke(get_vtable_cpo, std::allocator<T>)
  noexcept
  -> decltype( detail::vtable_for<T, OverloadsT...> )
  {
    return detail::vtable_for<T, OverloadsT...>;
  }

  template<typename AllocT,
           TITE_REQUIRES(is_tag_invocable_v<get_vtable_cpo, AllocT&&>)>
  constexpr auto operator()(AllocT&& t) const
  noexcept( noexcept( tag_invoke_fn{}(std::declval<get_vtable_cpo>(), std::forward<AllocT>(t)) ) )
  -> decltype( tag_invoke_fn{}(std::declval<get_vtable_cpo>(), std::forward<AllocT>(t)) )
  {
    return tag_invoke_fn{}(*this, std::forward<AllocT>(t));
  }
};

template<typename... OverloadsT>
constexpr static auto get_vtable = get_vtable_cpo<OverloadsT...>{};

} // namespace tite

#endif
