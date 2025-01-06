#ifndef TITE_DETAIL_VTABLE_HOLDER_HPP
#define TITE_DETAIL_VTABLE_HOLDER_HPP

#include "tite/detail/vtable.hpp"
#include "tite/type_traits/requires.hpp"

#include <memory>

#ifndef TITE_DIRECT_VTABLE_SIZE_MAX
#define TITE_DIRECT_VTABLE_SIZE_MAX 5
#endif

namespace tite {

namespace detail {

template<typename... OverloadsT>
struct indirect_vtable_holder {
public:
  template<typename T,
           typename AllocatorT=std::allocator<T>>
  constexpr static indirect_vtable_holder construct(AllocatorT&& allocator={})
  noexcept
  {
    return indirect_vtable_holder{get_vtable_pointer<T, OverloadsT...>(std::forward<AllocatorT>(allocator))};
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
           typename AllocatorT=std::allocator<T>>
  constexpr static direct_vtable_holder construct(AllocatorT&& allocator={})
  noexcept
  {
    return direct_vtable_holder{get_vtable<T, OverloadsT...>(std::forward<AllocatorT>(allocator))};
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
  direct_vtable_holder(vtable<OverloadsT...> v)
  noexcept
  : vtable_{v}
  {}

  vtable<OverloadsT...> vtable_;
};

/* had to do this because nvcc couldn't parse std::conditional_t */
template<typename... OverloadsT>
struct vtable_holder {
private:
  constexpr static bool check = sizeof...(OverloadsT) > TITE_DIRECT_VTABLE_SIZE_MAX;
public:
  using type = std::conditional_t<check,
                                  indirect_vtable_holder<OverloadsT...>,
                                  direct_vtable_holder<OverloadsT...>>;
};

template<typename... OverloadsT>
using vtable_holder_t = typename vtable_holder<OverloadsT...>::type;

} // namespace detail

} // namespace tite

#endif
