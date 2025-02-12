#ifndef TITE_DETAIL_REFERENCE_COUNTED_COPY_BASE_HPP
#define TITE_DETAIL_REFERENCE_COUNTED_COPY_BASE_HPP

#include "tite/detail/copy_cpo.hpp"
#include "tite/detail/deallocate_cpo.hpp"

#include <memory>
#include <atomic>

namespace tite {

struct copy_reference_counted_tag {};

constexpr static auto copy_reference_counted = copy_reference_counted_tag{};

namespace detail {

template<typename CRTPDerivedT>
struct reference_counted_copy_base {
protected:
  std::atomic_size_t count_;

  constexpr
  reference_counted_copy_base()
  : count_(static_cast<size_t>(1))
  {}

  constexpr
  reference_counted_copy_base(reference_counted_copy_base const& other)
  : count_{other.count_.load()}
  {}

  constexpr
  reference_counted_copy_base& operator=(reference_counted_copy_base const& other)
  {
    count_ = other.count_.load();
  }

  friend void tag_invoke(detail::deallocate_cpo,
                         CRTPDerivedT&& impl)
  noexcept
  {
    using base_type = typename CRTPDerivedT::reference_counted_copy_base;
    auto count = --impl.base_type::count_;
    if( ! count ) {
      using allocator_type = typename CRTPDerivedT::allocator_type;

      allocator_type alloc_copy = std::move(impl.alloc);

      std::allocator_traits<allocator_type>::destroy(alloc_copy, impl.ptr);
      std::allocator_traits<allocator_type>::deallocate(alloc_copy, impl.ptr, 1);
    }
  }

  friend void tag_invoke(detail::copy_cpo,
                         CRTPDerivedT const& from,
                         void*& to)
  {
    using base_type = typename CRTPDerivedT::reference_counted_copy_base;
    auto& derived_t = const_cast<CRTPDerivedT&>(from);
    derived_t.base_type::count_++;
    to = reinterpret_cast<void*>(derived_t.ptr);
  }
};

} // namespace detail

} // namespace tite

#endif
