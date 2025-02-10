#ifndef TITE_GPU_ALLOCATOR_HPP
#define TITE_GPU_ALLOCATOR_HPP

#include "tite/type_traits/requires.hpp"
#include "tite/type_traits/remove_cvref.hpp"
#include "tite/type_traits/allocator_tag.hpp"
#include "tite/detail/gpu_functions.hpp"
#include "tite/gpu_allocator_base.hpp"
#include "tite/type_traits/tag_invoke.hpp"

#include "tite/detail/void_ptr_cast.hpp"

#include <cstddef>
#include <type_traits>
#include <utility>
#include <memory>

namespace tite {

template<typename T>
#if ! defined(__NVCC__)
struct gpu_allocator : gpu_allocator_base<gpu_allocator<T>>{
#else
struct gpu_allocator {
#endif
  using host_allocator_type = std::allocator<T>;
  host_allocator_type host_allocator_;

  struct pointer_holder {
    T* host_ptr;
    T* gpu_ptr;
  };

  using value_type = T;
  using pointer = pointer_holder*;
  using size_type = size_t;

  /*
  template<typename host_allocator_type_,
           TITE_REQUIRES( not std::is_same<gpu_allocator, remove_cvref_t<host_allocator_type_>>::value ) >
  constexpr gpu_allocator(host_allocator_type_&& host_allocator) noexcept
  : host_allocator_{std::forward<host_allocator_type_>(host_allocator)}
  {}
  */

  constexpr
  gpu_allocator() {}

  template<typename U>
  constexpr gpu_allocator(gpu_allocator<U> const& other)
  : gpu_allocator{}
  {}

  template<typename IntT,
           TITE_REQUIRES( std::is_integral_v<IntT> )>
  constexpr pointer allocate(IntT n) {
    pointer retval = nullptr;
    TITE_GPU_ASSERT( titeGPUMalloc(&retval, n*sizeof(pointer_holder)) );
    for(IntT i=0; i<n; ++i) {
      T* h_ptr = std::allocator_traits<host_allocator_type>::allocate(host_allocator_, 1);
      TITE_GPU_ASSERT( titeGPUMemcpy(&(retval[i].host_ptr), &h_ptr, sizeof(T*), titeGPUMemcpyHostToDevice) );

      T* d_ptr = nullptr;
      TITE_GPU_ASSERT( titeGPUMalloc(&d_ptr, sizeof(T)) );
      TITE_GPU_ASSERT( titeGPUMemcpy(&(retval[i].gpu_ptr), &d_ptr, sizeof(T*), titeGPUMemcpyDeviceToDevice) );
    }

    return retval;
  }

	template<typename... ArgTs>
  constexpr void construct(pointer ptr, ArgTs&&... args) {
    T* h_ptr = nullptr;
    TITE_GPU_ASSERT( titeGPUMemcpy(&h_ptr, &(ptr->host_ptr), sizeof(T*), titeGPUMemcpyDeviceToHost) );
    std::allocator_traits<host_allocator_type>::construct(host_allocator_, h_ptr, std::forward<ArgTs>(args)...);

    T* d_ptr = nullptr;
    TITE_GPU_ASSERT( titeGPUMemcpy(&d_ptr, &(ptr->gpu_ptr), sizeof(T*), titeGPUMemcpyDeviceToHost) );
    TITE_GPU_ASSERT( titeGPUMemcpy(d_ptr, h_ptr, sizeof(T), titeGPUMemcpyDeviceToHost) );
	}

	constexpr void destroy(pointer ptr) {
    T* h_ptr = nullptr;
    TITE_GPU_ASSERT( titeGPUMemcpy(&h_ptr, &(ptr->host_ptr), sizeof(T*), titeGPUMemcpyDeviceToHost) );
    std::allocator_traits<host_allocator_type>::destroy(host_allocator_, h_ptr);
  }

  template<typename IntT,
           TITE_REQUIRES( std::is_integral_v<IntT> )>
  constexpr void deallocate(pointer ptr, IntT n) {
    for(IntT i=0; i<n; ++i) {
      T* h_ptr = nullptr;
      TITE_GPU_ASSERT( titeGPUMemcpy(&h_ptr, &(ptr[i].host_ptr), sizeof(T*), titeGPUMemcpyDeviceToHost) );
      std::allocator_traits<host_allocator_type>::deallocate(host_allocator_, h_ptr, 1);

      T* d_ptr = nullptr;
      TITE_GPU_ASSERT( titeGPUMemcpy(&d_ptr, &(ptr[i].gpu_ptr), sizeof(T*), titeGPUMemcpyDeviceToDevice) );
      TITE_GPU_ASSERT( titeGPUFree(d_ptr) );
    }

    TITE_GPU_ASSERT( titeGPUFree(ptr) );
  }

  constexpr
  friend auto tag_invoke(void_ptr_cast_cpo<T>, void* ptr, allocator_tag_t<gpu_allocator>)
  noexcept
  -> T&
  {
#if ! TITE_DEVICE_COMPILE()
    T* h_ptr = nullptr;
    TITE_GPU_ASSERT( titeGPUMemcpy(&h_ptr, &(static_cast<pointer_holder*>(ptr)->host_ptr), sizeof(T*), titeGPUMemcpyDeviceToHost) );
    return *h_ptr;
#else
    return *static_cast<T*>(static_cast<pointer_holder*>(ptr)->gpu_ptr);
#endif
  }

};

#if defined(__NVCC__)
template<typename T, typename U,
				 typename... OverloadsT>
constexpr auto tag_invoke(get_vtable_pointer_cpo<U,OverloadsT...>, allocator_tag_t<gpu_allocator<T>>)
noexcept
-> tite::detail::vtable<OverloadsT...>*
{
	tite::detail::vtable<OverloadsT...>* ptr = nullptr;
	TITE_GPU_ASSERT( titeGPUMemcpyFromSymbol(&ptr, detail::gpu_vtable_ptr<U,OverloadsT...>, sizeof(detail::gpu_vtable_ptr<U,OverloadsT...>)) );
	return ptr;
}

template<typename T, typename U,
				 typename... OverloadsT>
constexpr auto tag_invoke(get_vtable_cpo<U,OverloadsT...>, allocator_tag_t<gpu_allocator<T>>)
noexcept
-> tite::detail::vtable<OverloadsT...>
{
	tite::detail::vtable<OverloadsT...> vtable = detail::vtable_for<U,OverloadsT...>;
	TITE_GPU_ASSERT( titeGPUMemcpyFromSymbol(&vtable, detail::gpu_vtable_for<U,OverloadsT...>, sizeof(detail::gpu_vtable_for<U,OverloadsT...>)) );
	return vtable;
}
#endif

} // namespace tite

#endif
