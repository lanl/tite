#ifndef TITE_TYPE_TRAITS_TAG_INVOKE_HPP
#define TITE_TYPE_TRAITS_TAG_INVOKE_HPP

#include "tite/type_traits/void_t.hpp"

#include <type_traits>
#include <utility>

namespace tite {

inline namespace type_traits {

namespace tag_invoke_detail {
    void tag_invoke();

    struct tag_invoke_fn {
        template<typename CPO, typename... Args>
        constexpr auto operator()(CPO&& cpo, Args&&... args) const
            noexcept(noexcept(tag_invoke(std::declval<decltype(cpo)>(),
                                         std::declval<decltype(args)>()...)))
        -> decltype(tag_invoke(std::declval<decltype(cpo)>(),
                               std::declval<decltype(args)>()...))
        {
            return tag_invoke(std::forward<CPO>(cpo), std::forward<Args>(args)...);
        }
    };

    template<typename Dummy>
    struct tag_invoke_init {
      static tag_invoke_fn const tag_invoke;
    };

    template<typename Dummy>
    tag_invoke_fn const tag_invoke_init<Dummy>::tag_invoke={};
}

static constexpr auto& tag_invoke = tag_invoke_detail::tag_invoke_init<void>::tag_invoke;

template<typename CPO, typename... Args>
struct tag_invoke_result {
  using type = decltype(tag_invoke(std::declval<CPO &&>(),
                                   std::declval<Args &&>()...));
};

template<typename CPO, typename... Args>
using tag_invoke_result_t = typename tag_invoke_result<CPO, Args...>::type;

template<typename CPO, typename, typename... Args>
struct is_tag_invocable : std::false_type {};

template<typename CPO, typename... Args>
struct is_tag_invocable<CPO, type_traits::void_t<tag_invoke_result_t<CPO,Args...>>, Args...>
    : std::true_type {};

template<typename CPO, typename, typename... Args>
struct is_nothrow_tag_invocable : std::false_type {};

template<typename CPO, typename... Args>
struct is_nothrow_tag_invocable<CPO, type_traits::void_t<tag_invoke_result_t<CPO,Args...>>, Args...> {
    static constexpr bool value =
        noexcept(tag_invoke(std::declval<CPO &&>(),
                            std::declval<Args &&>()...));
};

template<typename CPO, typename... Args>
constexpr static bool is_tag_invocable_v = is_tag_invocable<CPO, void, Args...>::value;

template<typename CPO, typename... Args>
constexpr static bool is_nothrow_tag_invocable_v = is_nothrow_tag_invocable<CPO, void, Args...>::value;

} // inline namespace type_traits

} // namespace tite

#endif
