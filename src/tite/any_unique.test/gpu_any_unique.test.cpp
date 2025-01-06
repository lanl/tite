#include "tite.hpp"
#include "get_cpo.hpp"
#include "tite/host_held_gpu_allocator.hpp"

#include "get_cpo.hpp"

#include <cstdio>
#include <array>

struct blarg {
  template<typename IntT>
  constexpr auto operator[](IntT i) const {
    return 5;
  }
};

template<typename GettableT>
__global__ void test_get(GettableT const* const au, int* const result) {
  *result = get(*au, 1);
}

int run_any_unique_gpu_test() {
  int hint = 5;

  using tite::overload;
  using tite::any_unique;
  using tite::host_held_gpu_allocator;
  using tite::self;

  using erased_type = any_unique<overload<get_cpo, int(self const&, int)> >;

  erased_type const au1 = {blarg{}, host_held_gpu_allocator<int>{/* std::allocator<blarg>{} */ }};
  erased_type* d_ptr;

  TITE_GPU_ASSERT( titeGPUMalloc(&d_ptr, sizeof(erased_type)) );
  TITE_GPU_ASSERT( titeGPUMemcpy(d_ptr, &au1, sizeof(erased_type), titeGPUMemcpyHostToDevice) );

  int* d_int;
  TITE_GPU_ASSERT( titeGPUMalloc(&d_int, sizeof(int)) );

  test_get<<<1,1>>>(d_ptr, d_int);

  TITE_GPU_ASSERT( titeGPUMemcpy(&hint, d_int, sizeof(int), titeGPUMemcpyDeviceToHost) );

  TITE_GPU_ASSERT(titeGPUFree(d_ptr));
  TITE_GPU_ASSERT(titeGPUFree(d_int));

  return hint == 5;
}
