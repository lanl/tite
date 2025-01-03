#ifndef TITE_DETAIL_DEVICE_FUNCTIONS_HPP
#define TITE_DETAIL_DEVICE_FUNCTIONS_HPP

#if defined(__CUDACC__)

#include <cuda.h>
#define titeGPUMalloc cudaMalloc
#define titeGPUFree cudaFree
#define titeGPUMemcpy cudaMemcpy
#define titeGPUMemcpyFromSymbol cudaMemcpyFromSymbol
#define titeGPUMemcpyHostToDevice cudaMemcpyHostToDevice
#define titeGPUMemcpyDeviceToHost cudaMemcpyDeviceToHost
#define titeGPUSuccess cudaSuccess
#define titeGPUDeviceSynchronize cudaDeviceSynchronize

#elif defined(__HIPCC__)

#include <hip/hip_runtime.h>
#define titeGPUMalloc hipMalloc
#define titeGPUFree hipFree
#define titeGPUMemcpy hipMemcpy
#define titeGPUMemcpyFromSymbol hipMemcpyFromSymbol
#define titeGPUMemcpyHostToDevice hipMemcpyHostToDevice
#define titeGPUMemcpyDeviceToHost hipMemcpyDeviceToHost
#define titeGPUSuccess hipSuccess
#define titeGPUDeviceSynchronize hipDeviceSynchronize

#endif

#ifdef NDEBUG
#define TITE_GPU_ASSERT(x) [](auto&&){}(x)
#else
#include <cassert>
#define TITE_GPU_ASSERT(x) assert( (x) == titeGPUSuccess )
#endif

#endif
