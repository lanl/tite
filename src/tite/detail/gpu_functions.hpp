#ifndef TITE_DETAIL_GPU_FUNCTIONS_HPP
#define TITE_DETAIL_GPU_FUNCTIONS_HPP

#if defined(__CUDACC__)

#include <cuda.h>
#define titeGPUMalloc cudaMalloc
#define titeGPUFree cudaFree
#define titeGPUMemcpy cudaMemcpy
#define titeGPUMemcpyToSymbol cudaMemcpyToSymbol
#define titeGPUMemcpyFromSymbol cudaMemcpyFromSymbol
#define titeGPUMemcpyHostToDevice cudaMemcpyHostToDevice
#define titeGPUMemcpyDeviceToHost cudaMemcpyDeviceToHost
#define titeGPUMemcpyDeviceToDevice cudaMemcpyDeviceToDevice
#define titeGPUSuccess cudaSuccess
#define titeGPUDeviceSynchronize cudaDeviceSynchronize

#elif defined(__HIPCC__)

#include <hip/hip_runtime.h>
#define titeGPUMalloc hipMalloc
#define titeGPUFree hipFree
#define titeGPUMemcpy hipMemcpy
#define titeGPUMemcpyToSymbol hipMemcpyToSymbol
#define titeGPUMemcpyFromSymbol hipMemcpyFromSymbol
#define titeGPUMemcpyHostToDevice hipMemcpyHostToDevice
#define titeGPUMemcpyDeviceToHost hipMemcpyDeviceToHost
#define titeGPUMemcpyDeviceToDevice hipMemcpyDeviceToDevice
#define titeGPUSuccess hipSuccess
#define titeGPUDeviceSynchronize hipDeviceSynchronize

#endif

#ifdef NDEBUG
#define TITE_GPU_ASSERT(x) [](auto&&){}(x)
#else
#include <cassert>
#define TITE_GPU_ASSERT(x) assert( (x) == titeGPUSuccess )
#endif

#define TITE_DEVICE_COMPILE() \
  (defined(__CUDA_ARCH__) || defined(__HIP_DEVICE_COMPILE__))

#if TITE_DEVICE_COMPILE() && defined(__NVCC__)
#define TITE_CONSTEXPR_STATIC constexpr static __device__
#else
#define TITE_CONSTEXPR_STATIC constexpr static
#endif

#if defined(__CUDACC__) || defined(__HIPCC__)
#define TITE_HOST_FUNCTION __host__
#define TITE_DEVICE_FUNCTION __device__
#define TITE_HOST_DEVICE_FUNCTION __host__ __device__
#else
#define TITE_HOST_FUNCTION
#define TITE_DEVICE_FUNCTION
#define TITE_HOST_DEVICE_FUNCTION
#endif

#endif
