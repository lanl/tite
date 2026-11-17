#ifndef MODEL_HH
#define MODEL_HH

#include "model_options.hpp"
#include "tite/type_traits/remove_cvref.hpp"

#include <utility>

namespace repro {

template<typename T>
struct model {
  T some_type;
  model_options options;

  template<typename T_>
  constexpr
  model(T_&& some_type_, model_options const& options_)
  : some_type{std::forward<T_>(some_type_)}
  , options{options_}
  {}
};

template<typename T>
model(T&&, model_options const&) -> model<tite::remove_cvref_t<T>>;

} // namespace repro

#endif
