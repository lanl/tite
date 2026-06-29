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
#ifndef TITE_DETAIL_DEEP_COPY_BASE_HPP
#define TITE_DETAIL_DEEP_COPY_BASE_HPP

#include "tite/detail/copy_cpo.hpp"
#include "tite/detail/deallocate_cpo.hpp"

#include <memory>

namespace tite {

struct copy_deep_tag {};

constexpr static auto copy_deep = copy_deep_tag{};

namespace detail {

template<typename CRTPDerivedT>
struct deep_copy_base {
protected:
  friend void tag_invoke(detail::deallocate_cpo,
                         CRTPDerivedT&& impl)
  noexcept
  {
    using allocator_type = typename CRTPDerivedT::allocator_type;

    allocator_type alloc_copy = std::move(impl.alloc);

    std::allocator_traits<allocator_type>::destroy(alloc_copy, impl.ptr);
    std::allocator_traits<allocator_type>::deallocate(alloc_copy, impl.ptr, 1);
  }

  friend void tag_invoke(detail::copy_cpo,
                         CRTPDerivedT const& from,
                         void*& to)
  {
    using allocator_type = typename CRTPDerivedT::allocator_type;
    using allocation_pointer_type = typename CRTPDerivedT::allocation_pointer_type;

    allocator_type alloc_copy = from.alloc;
    to = std::allocator_traits<allocator_type>::allocate(alloc_copy, 1);
    try {
      std::allocator_traits<allocator_type>::construct(alloc_copy,
                                                       static_cast<allocation_pointer_type>(to),
                                                       CRTPDerivedT{std::allocator_arg,
                                                                     std::move(alloc_copy),
                                                                     static_cast<allocation_pointer_type>(to),
                                                                     from.value
                                                                    }
                                                      );
    } catch(...) {
      std::allocator_traits<allocator_type>::deallocate(alloc_copy,
                                                        static_cast<allocation_pointer_type>(to),
                                                        1);
      throw;
    }
  }
};

} // namespace detail

} // namespace tite

#endif
