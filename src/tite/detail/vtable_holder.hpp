#ifndef TITE_DETAIL_VTABLE_HOLDER_HPP
#define TITE_DETAIL_VTABLE_HOLDER_HPP

#include "tite/detail/vtable.hpp"
#include "tite/type_traits/requires.hpp"

#include <memory>

#ifndef TITE_DIRECT_VTABLE_SIZE_MAX
#define TITE_DIRECT_VTABLE_SIZE_MAX 1
#endif

namespace tite {

namespace detail {

template<typename... OverloadsT>
struct indirect_vtable_holder {
public:
  template<typename T,
           template<typename...> class VTablePointer=VTablePointerPolicy>
  constexpr static indirect_vtable_holder construct()
  noexcept
  {
    return indirect_vtable_holder{ VTablePointer<T,OverloadsT...>{}() };
  }

  constexpr vtable<OverloadsT...> const& operator*() const
  noexcept
  {
    return *vtable_;
  }

  constexpr vtable<OverloadsT...> const* operator->() const
  noexcept
  {
    return vtable_;
  }

private:
  constexpr
  indirect_vtable_holder(vtable<OverloadsT...> const* v)
  noexcept
  : vtable_{v}
  {}

  vtable<OverloadsT...> const* vtable_;
};

template<typename... OverloadsT>
struct direct_vtable_holder {
public:
  template<typename T,
           template<typename...> class VTablePointer=VTablePointerPolicy>
  constexpr static direct_vtable_holder construct()
  noexcept
  {
    return direct_vtable_holder{ *VTablePointer<T,OverloadsT...>{}() };
  }

  constexpr vtable<OverloadsT...> const& operator*() const
  noexcept
  {
    return vtable_;
  }

  constexpr vtable<OverloadsT...> const* operator->() const
  noexcept
  {
    return &vtable_;
  }

private:
  constexpr
  direct_vtable_holder(vtable<OverloadsT...> const& v)
  noexcept
  : vtable_{v}
  {}

  constexpr
  direct_vtable_holder(vtable<OverloadsT...>&& v)
  noexcept
  : vtable_{std::move(v)}
  {}

  vtable<OverloadsT...> vtable_;
};

template<typename...OverloadsT>
using vtable_holder_t = std::conditional_t<
                          (sizeof...(OverloadsT) > TITE_DIRECT_VTABLE_SIZE_MAX),
                          indirect_vtable_holder<OverloadsT...>,
                          direct_vtable_holder<OverloadsT...>>;

} // namespace detail

} // namespace tite

#endif
