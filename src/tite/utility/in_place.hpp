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
#ifndef TITE_UTILITY_IN_PLACE_HPP
#define TITE_UTILITY_IN_PLACE_HPP

// from https://en.cppreference.com/w/cpp/utility/in_place

#include <cstddef>

namespace tite {

struct in_place_t {
  explicit in_place_t() = default;
};

template<typename T>
struct in_place_type_t {
  explicit in_place_type_t() = default;
};

template<std::size_t I>
struct in_place_index_t {
  explicit in_place_index_t() = default;
};

} // namespace tite

#endif
