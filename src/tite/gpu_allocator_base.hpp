#ifndef TITE_GPU_ALLOCATOR_BASE_HPP
#define TITE_GPU_ALLOCATOR_BASE_HPP

#include "tite/detail/vtable.hpp"
#include "tite/detail/gpu_vtable.hpp"

namespace tite {

template<typename CRTPDerivedT>
struct gpu_allocator_base {
protected:
	template<typename U,
					 typename... OverloadsT>
	friend constexpr auto tag_invoke(get_vtable_pointer_cpo<U,OverloadsT...>, allocator_tag_t<CRTPDerivedT>)
	noexcept
	-> tite::detail::vtable<OverloadsT...>*
	{
		tite::detail::vtable<OverloadsT...>* ptr = nullptr;
		TITE_GPU_ASSERT( titeGPUMemcpyFromSymbol(&ptr, detail::gpu_vtable_ptr<U,OverloadsT...>, sizeof(detail::gpu_vtable_ptr<U,OverloadsT...>)) );
		return ptr;
	}

	template<typename U,
					 typename... OverloadsT>
	friend constexpr auto tag_invoke(get_vtable_cpo<U,OverloadsT...>, allocator_tag_t<CRTPDerivedT>)
	noexcept
	-> tite::detail::vtable<OverloadsT...>
	{
		tite::detail::vtable<OverloadsT...> vtable = detail::vtable_for<U,OverloadsT...>;
		TITE_GPU_ASSERT( titeGPUMemcpyFromSymbol(&vtable, detail::gpu_vtable_for<U,OverloadsT...>, sizeof(detail::gpu_vtable_for<U,OverloadsT...>)) );
		return vtable;
	}
};

} // namespace

#endif
