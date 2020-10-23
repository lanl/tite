#ifndef TITE_DETAIL_VTABLE_INVOKE_HPP
#define TITE_DETAIL_VTABLE_INVOKE_HPP

#include "tite/self.hpp"

namespace tite {

namespace detail {

template<typename T,
         typename CPOT,
         typename ReturnT,
         typename... ArgsT>
constexpr auto vtable_invoke(CPOT cpo,
                             replace_self_with_void_ptr_t<ArgsT>... args)
noexcept( noexcept( std::move(cpo)(replace_self<ArgsT>::get((ArgsT &&) args, std::declval<T&>())...) ) )
-> ReturnT
{
  void* self_ptr = extract_self<ArgsT...>{}(args...);
  T& obj = *static_cast<T*>(self_ptr);
  return std::move(cpo)(replace_self<ArgsT>::get((ArgsT &&) args, obj)...);
}

} // namespace detail

} // namespace tite

#endif
