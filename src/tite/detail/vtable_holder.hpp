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

template<typename AllocatorT, typename... OverloadsT>
struct indirect_vtable_holder {
public:
  template<typename T,
           template<typename...> class InvokePointerPolicy=default_invoke_pointer_policy,
           typename AllocatorT_,
           typename ReboundAllocatorT = typename std::allocator_traits<std::decay_t<AllocatorT_>>::template rebind_alloc<vtable<OverloadsT...>>,
           TITE_REQUIRES(std::is_constructible<ReboundAllocatorT, AllocatorT_&&>::value),
           TITE_REQUIRES(std::is_constructible<AllocatorT, ReboundAllocatorT&&>::value)>
  static indirect_vtable_holder construct(AllocatorT_&& allocator)
  noexcept( noexcept( std::allocator_traits<ReboundAllocatorT>::allocate(std::declval<ReboundAllocatorT&>(), 1) )
        and noexcept( indirect_vtable_holder{std::declval<vtable<OverloadsT...>*>(), std::declval<ReboundAllocatorT&&>()} ) )
  {
    ReboundAllocatorT vtable_allocator{std::forward<AllocatorT_>(allocator)};
    auto* vtable_ptr = std::allocator_traits<ReboundAllocatorT>::allocate(vtable_allocator, 1);
    std::allocator_traits<ReboundAllocatorT>::construct(vtable_allocator,
                                                        vtable_ptr,
                                                        vtable<OverloadsT...>::template construct<T,InvokePointerPolicy>());
    return indirect_vtable_holder{vtable_ptr, std::move(vtable_allocator)};
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

  constexpr
  indirect_vtable_holder(indirect_vtable_holder const& other)
  noexcept( std::is_nothrow_copy_constructible<AllocatorT>::value )
  : allocator_{ other.allocator_ }
  {
    vtable_ = std::allocator_traits<AllocatorT>::allocate(allocator_, 1);
    std::allocator_traits<AllocatorT>::construct(allocator_,
                                                 vtable_,
                                                 *other.vtable_);
  }

  constexpr
  indirect_vtable_holder(indirect_vtable_holder&& other)
  noexcept( std::is_nothrow_move_constructible<AllocatorT>::value )
  : vtable_{ std::move(other.vtable_) }
  , allocator_{ std::move(other.allocator_) }
  {
    other.vtable_ = nullptr;
  }

  constexpr
  indirect_vtable_holder& operator=(indirect_vtable_holder const& other)
  noexcept( std::is_nothrow_copy_assignable<AllocatorT>::value )
  {
    if( this == &other) return *this;
    *vtable_ = *other.vtable_;
    allocator_ = other.allocator_;
    return *this;
  }

  constexpr
  indirect_vtable_holder& operator=(indirect_vtable_holder&& other)
  noexcept( std::is_nothrow_move_assignable<AllocatorT>::value )
  {
    if( this == &other) return *this;
    std::allocator_traits<AllocatorT>::deallocate(allocator_, vtable_, 1);
    vtable_ = std::move(other.vtable_);
    other.vtable_ = nullptr;
    allocator_ = std::move(other.allocator_);
    return *this;
  }

  ~indirect_vtable_holder()
  noexcept
  {
    // a vtable is trivial (only holds pointers) so we don't need to destruct it?!?!
    // valgrind agrees...so far
    if( vtable_ != nullptr ) {
      std::allocator_traits<AllocatorT>::deallocate(allocator_, vtable_, 1);
    }
  }

private:
  constexpr
  indirect_vtable_holder(vtable<OverloadsT...>* v, AllocatorT&& allocator)
  noexcept( std::is_nothrow_move_constructible<AllocatorT>::value )
  : vtable_{v}
  , allocator_{std::move(allocator)}
  {}

  vtable<OverloadsT...>* vtable_;
  AllocatorT allocator_;
};

template<typename UnusedAllocatorT, typename... OverloadsT>
struct direct_vtable_holder {
public:
  template<typename T,
           template<typename...> class InvokePointerPolicy=default_invoke_pointer_policy,
           typename UnusedAllocatorT_>
  static direct_vtable_holder construct(UnusedAllocatorT_&&)
  noexcept
  {
    return direct_vtable_holder{ vtable<OverloadsT...>::template construct<T,InvokePointerPolicy>() };
  }

  constexpr vtable<OverloadsT...> const& operator*() const
  noexcept
  {
    return vtable_;
  }

  constexpr vtable<OverloadsT...> const* operator->() const
  noexcept
  {
    return std::addressof(vtable_);
  }

private:
  constexpr
  direct_vtable_holder(vtable<OverloadsT...>&& v)
  noexcept
  : vtable_{std::move(v)}
  {}

  vtable<OverloadsT...> vtable_;
};

template<typename AllocatorT, typename...OverloadsT>
using vtable_holder_t = std::conditional_t<
                          (sizeof...(OverloadsT) > TITE_DIRECT_VTABLE_SIZE_MAX),
                          indirect_vtable_holder<AllocatorT, OverloadsT...>,
                          direct_vtable_holder<AllocatorT, OverloadsT...>>;

} // namespace detail

} // namespace tite

#endif
