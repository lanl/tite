#include "model.hpp"
#include "model_te.hpp"

#include "tite/detail/gpu_functions.hpp"

#include <cstdio>

namespace repro {

struct foobarbaz {};

__global__
void static check_it(model_te m) {
  auto options = get_model_options(m);

  printf("foo=%lf; bar=%lf\n", options.foo, options.bar);
}

model_te make_model_te(double f, double b) {
  auto model = repro::model_te{repro::model{ foobarbaz{}, repro::model_options{f, b} }, tite::gpu_allocator<std::byte>{}};

  std::printf("=== BEGIN CHECKING MODEL ===\n");
  check_it<<<1,1>>>(model);
  TITE_GPU_ASSERT( titeGPUDeviceSynchronize() );
  std::printf("=== END CHECKING MODEL ===\n");

  return model;
}

} // namespace repro
