#ifndef TITE_TYPE_TRAITS_REMOVE_CVREF_HPP
#define TITE_TYPE_TRAITS_REMOVE_CVREF_HPP

#include <type_traits>

namespace tite {

template<typename T>
struct remove_cvref {
  using type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
};

template<typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

}

#endif
