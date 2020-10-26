#ifndef TITE_ANY_UNIQUE_HPP
#define TITE_ANY_UNIQUE_HPP

#include "tite/detail/with_forwarding_tag_invoke.hpp"
#include "tite/detail/with_type_erased_tag_invoke.hpp"
#include "tite/detail/deallocate.hpp"
#include "tite/vtable.hpp"
#include "tite/type_traits.hpp"
#include "tite/utility/in_place.hpp"

#include <type_traits>
#include <memory>

namespace tite {

template <typename VTableAllocatorT,
          typename... OverloadsT>
class any_unique
    : private detail::
          with_type_erased_tag_invoke<any_unique<OverloadsT...>, OverloadsT>... {
private:
  using vtable_holder_t =
      detail::vtable_holder_t<VTableAllocatorT, detail::deallocate, OverloadsT...>;

public:
  template<typename ConcreteT,
           typename AllocatorT,
           typename... ArgsT,
           TITE_REQUIRES(std::is_constructible<ConcreteT, ArgsT&&...>::value),
           TITE_REQUIRES(is_type_erasable_v<ConcreteT, OverloadsT...>)>
  explicit any_unique(std::allocator_arg_t,
                      AllocatorT alloc,
                      in_place_type_t<ConcreteT>,
                      ArgsT&&... args)
  : vtable_{vtable_holder_t::template construct<concrete_impl<ConcreteT, AllocatorT>>(alloc)}
  {
    using concrete_type = concrete_impl<ConcreteT, AllocatorT>;
    using allocator_type = typename concrete_type::allocator_type;
    using allocator_traits = std::allocator_traits<allocator_type>;
    allocator_type typedAllocator{std::move(alloc)};
    auto ptr = allocator_traits::allocate(typedAllocator, 1);
    try {
      allocator_traits::construct(typedAllocator, ptr,
          concrete_type{std::allocator_arg, typedAllocator, std::forward<ArgsT>(args)...});
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
  any_unique(ConcreteT&& concrete, AllocatorT alloc)
  : any_unique(std::allocator_arg_t{},
               std::move(alloc),
               in_place_type_t<remove_cvref_t<ConcreteT>>{},
               std::forward<ConcreteT>(concrete))
  {}

  template<typename ConcreteT,
           typename... ArgsT,
           TITE_REQUIRES(std::is_constructible<remove_cvref_t<ConcreteT>, ArgsT&&...>::value),
           TITE_REQUIRES(is_type_erasable_v<ConcreteT, OverloadsT...>)>
  explicit any_unique(in_place_type_t<ConcreteT> tag, ArgsT&&... args)
  : any_unique(std::allocator_arg,
               std::allocator<unsigned char>{},
               tag,
               std::forward<ArgsT>(args)...)
  {}

  template<typename ConcreteT,
           TITE_REQUIRES(not is_specialization_of_v<in_place_type_t, ConcreteT>),
           TITE_REQUIRES(not std::is_convertible<remove_cvref_t<ConcreteT>, any_unique>::value),
           TITE_REQUIRES(std::is_constructible<remove_cvref_t<ConcreteT>, ConcreteT&&>::value),
           TITE_REQUIRES(is_type_erasable_v<ConcreteT, OverloadsT...>)>
  any_unique(ConcreteT&& concrete)
  : any_unique{in_place_type_t<remove_cvref_t<ConcreteT>>{},
               std::forward<ConcreteT>(concrete)}
  {}

  any_unique(any_unique&& other) noexcept
      : impl_(std::exchange(other.impl_, nullptr)), vtable_(other.vtable_) {}

  ~any_unique() {
    if (impl_ != nullptr) {
      auto deallocateFn = vtable_->template fn<detail::deallocate>();
      deallocateFn(detail::deallocate{}, impl_);
    }
  }

 private:
  template <typename ConcreteT, typename AllocatorT>
  struct concrete_impl final : private detail::with_forwarding_tag_invoke<
                                          concrete_impl<ConcreteT, AllocatorT>,
                                          OverloadsT>... {
    using allocator_type = typename std::allocator_traits<AllocatorT>::template rebind_alloc<concrete_impl>;

    template <typename... ArgsT>
    explicit concrete_impl(std::allocator_arg_t,
                           allocator_type alloc,
                           ArgsT&&... args)
    noexcept( std::is_nothrow_move_constructible<allocator_type>::value
          and std::is_nothrow_constructible<ConcreteT, ArgsT&&...>::value)
    : value{std::forward<ArgsT>(args)...}
    , alloc{std::move(alloc)}
    {}

    friend void tag_invoke(detail::deallocate,
                           concrete_impl&& impl)
    noexcept
    {
      allocator_type allocCopy = std::move(impl.alloc);
      std::allocator_traits<allocator_type>::destroy(allocCopy, std::addressof(impl));
      std::allocator_traits<allocator_type>::deallocate(allocCopy, std::addressof(impl), 1);
    }

    ConcreteT value;
    allocator_type alloc;
  };

  template <typename Derived, typename CPO, typename Sig>
  friend struct detail::with_type_erased_tag_invoke;

  const auto& get_vtable() const noexcept {
    return vtable_;
  }

  void* get_object_address() const noexcept {
    return impl_;
  }

  void* impl_;
  vtable_holder_t vtable_;
};

} // namespace tite

#endif
