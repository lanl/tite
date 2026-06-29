/*
 * © 2025. Triad National Security, LLC. All rights reserved.
 * This program was produced under U.S. Government contract 89233218CNA000001
 * for Los Alamos National Laboratory (LANL), which is operated by Triad
 * National Security, LLC for the U.S. Department of Energy/National Nuclear
 * Security Administration. All rights in the program are reserved by Triad
 * National Security, LLC, and the U.S. Department of Energy/National Nuclear
 * Security Administration. The Government is granted for itself and others
 * acting on its behalf a nonexclusive, paid-up, irrevocable worldwide license
 * in this material to reproduce, prepare derivative works, distribute copies
 * to the public, perform publicly and display publicly, and to permit others
 * to do so.
 */
#ifndef TITE_ANY_UNIQUE_HPP
#define TITE_ANY_UNIQUE_HPP

#include "tite/detail/with_forwarding_tag_invoke.hpp"
#include "tite/detail/with_type_erased_tag_invoke.hpp"
#include "tite/detail/gpu_functions.hpp"
#include "tite/detail/deallocate_cpo.hpp"
#include "tite/detail/copy_cpo.hpp"
#include "tite/detail/copy_base.hpp"
#include "tite/vtable.hpp"
#include "tite/type_traits.hpp"
#include "tite/utility/in_place.hpp"

#include <type_traits>
#include <memory>

namespace tite {

template<typename... OverloadsT>
class any_unique
    : private detail::with_type_erased_tag_invoke<any_unique<OverloadsT...>, OverloadsT>... {
private:
  using vtable_holder_t = detail::vtable_holder_t<OverloadsT...>;
  using special_vtable_holder_t = detail::vtable_holder_t<detail::copy_cpo, detail::deallocate_cpo>;

public:
  template<typename ConcreteT,
           typename AllocatorT,
           typename CopyTagT,
           typename... ArgsT,
           TITE_REQUIRES(std::is_constructible<ConcreteT, ArgsT&&...>::value),
           TITE_REQUIRES(is_type_erasable_v<ConcreteT, OverloadsT...>)>
  explicit any_unique(std::allocator_arg_t,
                      AllocatorT alloc,
                      CopyTagT,
                      in_place_type_t<ConcreteT>,
                      ArgsT&&... args)
  : vtable_{vtable_holder_t::template construct<concrete_impl<ConcreteT, AllocatorT, CopyTagT>>(
              allocator_tag_v<typename concrete_impl<ConcreteT, AllocatorT, CopyTagT>::allocator_type>)}
  , special_vtable_{special_vtable_holder_t::template construct<concrete_impl<ConcreteT, AllocatorT, CopyTagT>>(
              allocator_tag_v<std::allocator<concrete_impl<ConcreteT, AllocatorT, CopyTagT>>>)}
  {
    using concrete_type = concrete_impl<ConcreteT, AllocatorT, CopyTagT>;
    using allocator_type = typename concrete_type::allocator_type;
    using allocator_traits = std::allocator_traits<allocator_type>;
    allocator_type typedAllocator{std::move(alloc)};
    auto ptr = allocator_traits::allocate(typedAllocator, 1);
    try {
      allocator_traits::construct(typedAllocator, ptr,
          concrete_type{std::allocator_arg, std::move(typedAllocator), ptr, std::forward<ArgsT>(args)...});
    } catch (...) {
      allocator_traits::deallocate(typedAllocator, ptr, 1);
      throw;
    }
    impl_ = static_cast<void*>(ptr);
  }

  template<typename ConcreteT,
           typename AllocatorT,
           TITE_REQUIRES(std::is_constructible<remove_cvref_t<ConcreteT>, ConcreteT&&>::value),
           TITE_REQUIRES(not std::is_same<std::allocator_arg_t, std::decay_t<ConcreteT>>::value ),
           TITE_REQUIRES(not is_specialization_of<in_place_type_t, std::decay_t<ConcreteT>>::value),
           TITE_REQUIRES(is_type_erasable_v<ConcreteT, OverloadsT...>)>
  constexpr
  any_unique(ConcreteT&& concrete, AllocatorT alloc)
  : any_unique(std::allocator_arg_t{},
               std::move(alloc),
               detail::copy_default_tag{},
               in_place_type_t<remove_cvref_t<ConcreteT>>{},
               std::forward<ConcreteT>(concrete))
  {}

  template<typename ConcreteT,
           typename AllocatorT,
           typename CopyTagT,
           TITE_REQUIRES(std::is_constructible<remove_cvref_t<ConcreteT>, ConcreteT&&>::value),
           TITE_REQUIRES(not std::is_same<std::allocator_arg_t, std::decay_t<ConcreteT>>::value ),
           TITE_REQUIRES(not is_specialization_of<in_place_type_t, std::decay_t<ConcreteT>>::value),
           TITE_REQUIRES(is_type_erasable_v<ConcreteT, OverloadsT...>)>
  constexpr
  any_unique(ConcreteT&& concrete, AllocatorT alloc, CopyTagT copy_tag)
  : any_unique(std::allocator_arg_t{},
               std::move(alloc),
               copy_tag,
               in_place_type_t<remove_cvref_t<ConcreteT>>{},
               std::forward<ConcreteT>(concrete))
  {}

  template<typename ConcreteT,
           typename... ArgsT,
           TITE_REQUIRES(std::is_constructible<remove_cvref_t<ConcreteT>, ArgsT&&...>::value),
           TITE_REQUIRES(is_type_erasable_v<ConcreteT, OverloadsT...>)>
  constexpr
  explicit any_unique(in_place_type_t<ConcreteT> tag, ArgsT&&... args)
  : any_unique(std::allocator_arg,
               std::allocator<unsigned char>{},
               detail::copy_default_tag{},
               tag,
               std::forward<ArgsT>(args)...)
  {}

  template<typename ConcreteT,
           TITE_REQUIRES(not is_specialization_of_v<in_place_type_t, ConcreteT>),
           TITE_REQUIRES(not std::is_convertible<remove_cvref_t<ConcreteT>, any_unique>::value),
           TITE_REQUIRES(std::is_constructible<remove_cvref_t<ConcreteT>, ConcreteT&&>::value),
           TITE_REQUIRES(is_type_erasable_v<ConcreteT, OverloadsT...>)>
  constexpr
  any_unique(ConcreteT&& concrete)
  : any_unique{in_place_type_t<remove_cvref_t<ConcreteT>>{},
               std::forward<ConcreteT>(concrete)}
  {}

  constexpr
  any_unique(any_unique const& other)
  : impl_{ other.impl_ }
  , vtable_{ other.vtable_ }
	, special_vtable_{ other.special_vtable_ }
	{
#if ! TITE_DEVICE_COMPILE()
    auto copyFn = special_vtable_->template fn<detail::copy_cpo>();
    copyFn(detail::copy_cpo{}, other.impl_, impl_);
#endif
  }

  constexpr
  any_unique(any_unique&& other) noexcept
  : impl_(std::exchange(other.impl_, nullptr))
  , vtable_(std::move(other.vtable_))
  , special_vtable_(std::move(other.special_vtable_))
  {}

  constexpr
  any_unique& operator=(any_unique const& other) {
    if( this == std::addressof(other) ) return *this;

		vtable_ = other.vtable_;
		special_vtable_ = other.special_vtable_;

#if TITE_DEVICE_COMPILE()
		impl_ = other.impl_;
#else
    if (impl_ != nullptr) {
      auto deallocateFn = special_vtable_->template fn<detail::deallocate_cpo>();
      deallocateFn(detail::deallocate_cpo{}, impl_);
    }

    auto copyFn = special_vtable_->template fn<detail::copy_cpo>();
    copyFn(detail::copy_cpo{}, other.impl_, impl_);
#endif

    return *this;
  }

  constexpr
  any_unique& operator=(any_unique&& other) {
    if( this == std::addressof(other) ) return *this;

    if (impl_ != nullptr) {
      auto deallocateFn = vtable_->template fn<detail::deallocate_cpo>();
      deallocateFn(detail::deallocate_cpo{}, impl_);
    }

    impl_ = std::exchange(other.impl_, nullptr);
    vtable_ = std::move(other.vtable_);
    special_vtable_ = std::move(other.special_vtable_);
    return *this;
  }

  TITE_HOST_DEVICE_FUNCTION
  ~any_unique() {
    if (impl_ != nullptr) {
      auto deallocateFn = special_vtable_->template fn<detail::deallocate_cpo>();
      deallocateFn(detail::deallocate_cpo{}, impl_);
    }
  }

public:
  template <typename ConcreteT, typename AllocatorT, typename CopyBaseTagT>
  struct concrete_impl final : detail::copy_base_t<CopyBaseTagT, concrete_impl<ConcreteT, AllocatorT, CopyBaseTagT>, OverloadsT...>,
                               private detail::with_forwarding_tag_invoke<
                                         concrete_impl<ConcreteT, AllocatorT, CopyBaseTagT>,
                                         OverloadsT>... {
    using copy_base = detail::copy_base_t<CopyBaseTagT, concrete_impl<ConcreteT, AllocatorT, CopyBaseTagT>, OverloadsT...>;
    using allocator_type = typename std::allocator_traits<AllocatorT>::template rebind_alloc<concrete_impl>;
    using allocation_pointer_type = typename std::allocator_traits<allocator_type>::pointer;

    template <typename... ArgsT>
    constexpr
    explicit concrete_impl(std::allocator_arg_t,
                           allocator_type&& alloc,
                           allocation_pointer_type ptr,
                           ArgsT&&... args)
    noexcept( std::is_nothrow_move_constructible<allocator_type>::value
          and std::is_nothrow_constructible<ConcreteT, ArgsT&&...>::value)
    : copy_base{}
    , value{std::forward<ArgsT>(args)...}
    , alloc{std::move(alloc)}
    , ptr{ptr}
    {}

    ConcreteT value;
    allocator_type alloc;
    allocation_pointer_type ptr;
  };

  template <typename Derived, typename CPO, typename Sig>
  friend struct detail::with_type_erased_tag_invoke;

public:
  constexpr
  const auto& get_vtable() const noexcept {
    return vtable_;
  }

  constexpr
  const auto& get_special_vtable() const noexcept {
    return special_vtable_;
  }

  constexpr
  void* get_object_address() const noexcept {
    return impl_;
  }

  void* impl_;
  vtable_holder_t vtable_;
  special_vtable_holder_t special_vtable_;
};

} // namespace tite

#endif
