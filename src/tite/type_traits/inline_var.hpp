#ifndef TITE_UTILITY_INLINE_VAR_HPP
#define TITE_UTILITY_INLINE_VAR_HPP

#define TITE_INLINEVAR( type, name, ... ) \
  namespace name##_tite_inline_var { \
    template<typename Dummy> \
    struct name##_tite_inline_var_struct { \
      static type name; \
    }; \
    template<typename Dummy> \
    type name##_tite_inline_var_struct<Dummy>::name = {__VA_ARGS__}; \
  } \
  constexpr static type& name = name##_tite_inline_var::name##_tite_inline_var_struct<void>::name;

#endif
