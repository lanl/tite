#ifndef MODEL_TE_HH
#define MODEL_TE_HH

#include "model_options.hpp"

#define TITE_DIRECT_VTABLE_SIZE_MAX 0

#include <tite/tite.hpp>

namespace repro {

struct get_model_options_cpo : tite::cpo_base<get_model_options_cpo> {
private:
template<typename ModelT, typename... ArgTs>
constexpr friend
auto tag_invoke(get_model_options_cpo, ModelT&& model, ArgTs&&...)
-> decltype(std::forward<ModelT>(model).options)
{
  return std::forward<ModelT>(model).options;
}

};

using model_te = tite::any_unique<
		  tite::overload<get_model_options_cpo, model_options(tite::self const&)>
		 >;

TITE_CONSTEXPR_STATIC auto get_model_options = get_model_options_cpo{};

} // namespace repro

#endif
