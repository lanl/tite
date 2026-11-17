#include "model.hpp"
#include "model_te.hpp"

#include <cstdio>

namespace repro {
extern model_te make_model_te(double f, double b);

extern void apply_model(model_te const& m);
}

int main () {
  auto m = repro::make_model_te(5.0, 25.0);

  repro::apply_model(m);
}
