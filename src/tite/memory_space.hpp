#ifndef TITE_MEMORY_SPACE_HPP
#define TITE_MEMORY_SPACE_HPP

#include "tite/detail/vtable.hpp"

#include <memory>

namespace tite {

template<template<typename> class AllocatorC,
         template<typename...> class VTablePointerC>
struct memory_space {
  template<typename T>
  using allocator = AllocatorC<T>;

  template<typename... ArgsT>
  using vtable_pointer_policy = VTablePointerC<ArgsT...>;
};

using default_memory_space = memory_space<std::allocator,
                                          detail::default_vtable_pointer_policy>;

} // namespace tite

#endif
