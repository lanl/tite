#ifndef TITE_MEMORY_SPACE_POLICY_HPP
#define TITE_MEMORY_SPACE_POLICY_HPP

#include "tite/default_invoke_pointer_policy.hpp"

#include <memory>

namespace tite {

template<template<typename> class AllocatorC,
         template<typename...> class VTablePointerC>
struct memory_space_policy {
  template<typename T>
  using allocator = AllocatorC<T>;

  template<typename... ArgsT>
  using vtable_pointer_policy = VTablePointerC<ArgsT...>;
};

using default_memory_policy = memory_space_policy<std::allocator,
                                                  default_invoke_pointer_policy>;

} // namespace tite

#endif
