#ifndef TITE_VTABLE_HPP
#define TITE_VTABLE_HPP

#include "tite/self.hpp"
#include "tite/vtable/default_invoke_pointer_policy.hpp"
#include "tite/type_traits/requires.hpp"

#include <memory>
#include <type_traits>
#include <utility>

#ifndef TITE_MAX_DIRECT_VTABLES
#define TITE_MAX_DIRECT_VTABLES 1
#endif

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
    return vtable_entry<OverloadT>::fn();
  }

private:
  explicit constexpr
  vtable(vtable_entry<OverloadsT>... entries)
  : vtable_entry<OverloadsT>{entries}...
  {}
};

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
  noexcept( noexcept( std::declval<ReboundAllocatorT&>().allocate(1) )
        and noexcept( indirect_vtable_holder{std::declval<vtable<OverloadsT...>*>(), std::declval<ReboundAllocatorT&&>()} ) )
  {
    ReboundAllocatorT vtable_allocator{std::forward<AllocatorT_>(allocator)};
    auto* vtable_ptr = vtable_allocator.allocate(1);
    new (vtable_ptr) vtable<OverloadsT...>{vtable<OverloadsT...>::template construct<T,InvokePointerPolicy>()};
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
    vtable_ = allocator_.allocate(1);
    new (vtable_) vtable<OverloadsT...>{*other.vtable_};
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
    allocator_.deallocate(vtable_, 1);
    vtable_ = std::move(other.vtable_);
    other.vtable_ = nullptr;
    allocator_ = std::move(other.allocator_);
    return *this;
  }

  ~indirect_vtable_holder()
  noexcept
  {
    // a vtable is trivial (only holds pointers) so we don't need to destruct it...I think?!?!
    if( vtable_ != nullptr ) {
      allocator_.deallocate(vtable_, 1);
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

  constexpr vtable<OverloadsT...> const& operator->() const
  noexcept
  {
    return vtable_;
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
                          (sizeof...(OverloadsT) > TITE_MAX_DIRECT_VTABLES),
                          indirect_vtable_holder<AllocatorT, OverloadsT...>,
                          direct_vtable_holder<AllocatorT, OverloadsT...>>;

} // namespace detail

} // namespace tite

#endif
