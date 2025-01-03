#ifndef TITE_DEVICE_ALLOCATOR_HPP
#define TITE_DEVICE_ALLOCATOR_HPP

#include "tite/type_traits/requires.hpp"
#include "tite/detail/vtable.hpp"
#include "tite/detail/gpu_functions.hpp"
#include "tite/detail/gpu_vtable.hpp"
#include "tite/detail/gpu_vtable.hpp"
#include "tite/type_traits/tag_invoke.hpp"

#include <cstddef>
#include <type_traits>
#include <utility>

namespace tite {

namespace detail {

template<typename T,
         typename... ArgTs>
__global__ void construct(T* const ptr, ArgTs... args) {
  new (ptr) T{args...};
}

template<typename T>
__global__ void destroy(T* const ptr) {
  ptr->~T();
}

} // namespace detail

template<typename T>
struct gpu_allocator {
  using value_type = T;
  using pointer = T*;
  using size_type = size_t;

  template<typename IntT,
           TITE_REQUIRES( std::is_integral<IntT>::value )>
  constexpr pointer allocate(size_t n) const {
    pointer retval = nullptr;
    TITE_GPU_ASSERT( titeGPUMalloc(&retval, n*sizeof(value_type)) );
    return retval;
  }

	template<typename U,
					 typename... ArgTs>
  constexpr void construct(U* const ptr, ArgTs&&... args) const {
		detail::construct<<<1,1>>>(ptr, std::forward<ArgTs>(args)...);
		titeGPUDeviceSynchronize();
	}

	template<typename U>
	constexpr void destroy(U* const ptr) const {
		detail::destroy<<<1,1>>>(ptr);
	}

  template<typename IntT,
           TITE_REQUIRES( std::is_integral<IntT>::value )>
  constexpr void deallocate(pointer ptr, size_t n) const {
		TITE_GPU_ASSERT( titeGPUFree(ptr) );
	}

	template<typename U,
					 typename... OverloadsT>
	friend constexpr auto tag_invoke(get_vtable_pointer_cpo<U,OverloadsT...>, gpu_allocator<U>)
	noexcept
	-> tite::detail::vtable<OverloadsT...>*
	{
		tite::detail::vtable<OverloadsT...>* ptr = nullptr;
		TITE_GPU_ASSERT( titeGPUMemcpyFromSymbol(&ptr, detail::gpu_vtable_ptr<U,OverloadsT...>, sizeof(detail::gpu_vtable_ptr<U,OverloadsT...>)) );
		return ptr;
	}

	template<typename U,
					 typename... OverloadsT>
	friend constexpr auto tag_invoke(get_vtable_cpo<U,OverloadsT...>, gpu_allocator<U>)
	noexcept
	-> tite::detail::vtable<OverloadsT...>
	{
		tite::detail::vtable<OverloadsT...> vtable = detail::vtable_for<U,OverloadsT...>;
		TITE_GPU_ASSERT( titeGPUMemcpyFromSymbol(&vtable, detail::gpu_vtable_for<U,OverloadsT...>, sizeof(detail::gpu_vtable_for<U,OverloadsT...>)) );
		return vtable;
	}

};

} // namespace tite

#endif
