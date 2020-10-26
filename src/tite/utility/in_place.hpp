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
