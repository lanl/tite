#include "tite.hpp"
#include "tite/host_held_gpu_allocator.hpp"

#include <cstdio>

#include <catch2/catch_test_macros.hpp>

template<typename PointerT>
__global__ void get_value(PointerT const * const ptr, int* const d_ptr) {
  *d_ptr = *(ptr->gpu_ptr);
}

TEST_CASE("host_held_gpu_allocator",
          "[host_held_gpu_allocator]") {

  using tite::host_held_gpu_allocator;

  auto alloc = host_held_gpu_allocator<int>{ /*std::allocator<int>{}*/ };

  auto ptr = alloc.allocate(1);
  alloc.construct(ptr, 5);

  int* h_ptr;
  TITE_GPU_ASSERT( titeGPUMemcpy(&h_ptr, &(ptr->host_ptr), sizeof(int*), titeGPUMemcpyDeviceToHost) );
  CHECK(*h_ptr == 5);

  int* d_ptr;
  TITE_GPU_ASSERT( titeGPUMalloc(&d_ptr, sizeof(int)) );
  get_value<<<1,1>>>(ptr, d_ptr);
  *h_ptr = 0;
  TITE_GPU_ASSERT( titeGPUMemcpy(h_ptr, d_ptr, sizeof(int), titeGPUMemcpyDeviceToHost) );
  CHECK(*h_ptr == 5);

  alloc.destroy(ptr);
  alloc.deallocate(ptr, 1);
}
