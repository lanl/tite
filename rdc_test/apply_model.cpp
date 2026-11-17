#include "model_te.hpp"

#include "tite/detail/gpu_functions.hpp"

#include <cstdio>

namespace repro {

__global__
void static apply_it(model_te m) {
  auto options = get_model_options(m);

  printf("foo=%lf; bar=%lf\n", options.foo, options.bar);
}

void apply_model(model_te const& m) {
  std::printf("=== BEGIN APPLYING MODEL ===\n");
  apply_it<<<1,1>>>(m);
  TITE_GPU_ASSERT( titeGPUDeviceSynchronize() );
  std::printf("=== END APPLYING MODEL ===\n");
}

} // namespace repro
