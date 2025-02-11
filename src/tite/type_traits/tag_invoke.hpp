#ifndef TITE_TYPE_TRAITS_TAG_INVOKE_HPP
#define TITE_TYPE_TRAITS_TAG_INVOKE_HPP

#include "tite/type_traits/void_t.hpp"

#include <type_traits>
#include <utility>

namespace tite {

namespace tag_invoke_detail {
  struct tag_invoke_fn {
    template<typename CPO, typename... Args>
    constexpr auto operator()(CPO&& cpo, Args&&... args) const
    noexcept(noexcept(tag_invoke(std::declval<CPO&&>(),
                                 std::declval<Args&&>()...)))
    -> decltype(tag_invoke(std::declval<CPO&&>(),
                           std::declval<Args&&>()...))
    {
        return tag_invoke(std::forward<CPO>(cpo), std::forward<Args>(args)...);
    }
  };
}

using tag_invoke_fn = tag_invoke_detail::tag_invoke_fn;

template<typename CPO, typename... Args>
struct tag_invoke_result {
  using type = decltype(tag_invoke_fn{}(std::declval<CPO &&>(),
                                        std::declval<Args &&>()...));
};

template<typename CPO, typename... Args>
using tag_invoke_result_t = typename tag_invoke_result<CPO, Args...>::type;

template<typename CPO, typename... Args>
struct is_tag_invocable {
private:
  template<typename CPO_, typename = void>
  struct impl : std::false_type {};

  template<typename CPO_>
  struct impl<CPO_, void_t<tag_invoke_result_t<CPO_, Args...>>> : std::true_type {};

public:
  constexpr static bool value = impl<CPO>::value;
};

template<typename CPO, typename... Args>
struct is_nothrow_tag_invocable {
private:
  template<typename CPO_, typename = void>
  struct impl : std::false_type {};

  template<typename CPO_>
  struct impl<CPO_, void_t<tag_invoke_result_t<CPO_, Args...>>> {
    constexpr static bool value = noexcept( tag_invoke_fn{}(std::declval<CPO &&>(),
                                                            std::declval<Args &&>()...) );
  };

public:
  constexpr static bool value = impl<CPO>::value;
};

template<typename CPO, typename... Args>
constexpr static bool is_tag_invocable_v = is_tag_invocable<CPO, Args...>::value;

template<typename CPO, typename... Args>
constexpr static bool is_nothrow_tag_invocable_v = is_nothrow_tag_invocable<CPO, Args...>::value;

} // namespace tite

#endif
