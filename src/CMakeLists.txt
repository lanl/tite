set(prefix $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>$<INSTALL_INTERFACE:include/tite>)
target_sources(tite INTERFACE
FILE_SET
  tite_headers

TYPE
  HEADERS

FILES
  ${prefix}/tite/any_unique.hpp
  ${prefix}/tite/cpo_base.hpp
  ${prefix}/tite/gpu_allocator.hpp
  ${prefix}/tite/gpu_allocator_base.hpp
  ${prefix}/tite/overload.hpp
  ${prefix}/tite/self.hpp
  ${prefix}/tite/tite.hpp
  ${prefix}/tite/type_traits.hpp
  ${prefix}/tite/utility.hpp
  ${prefix}/tite/vtable.hpp

  # detail
  ${prefix}/tite/detail/copy_base.hpp
  ${prefix}/tite/detail/copy_cpo.hpp
  ${prefix}/tite/detail/deallocate_cpo.hpp
  ${prefix}/tite/detail/deep_copy_base.hpp
  ${prefix}/tite/detail/gpu_functions.hpp
  ${prefix}/tite/detail/gpu_vtable.hpp
  ${prefix}/tite/detail/reference_counted_copy_base.hpp
  ${prefix}/tite/detail/void_ptr_cast.hpp
  ${prefix}/tite/detail/vtable.hpp
  ${prefix}/tite/detail/vtable_entry.hpp
  ${prefix}/tite/detail/vtable_holder.hpp
  ${prefix}/tite/detail/vtable_invoke.hpp
  ${prefix}/tite/detail/with_forwarding_tag_invoke.hpp
  ${prefix}/tite/detail/with_type_erased_tag_invoke.hpp

  # type_traits
  ${prefix}/tite/type_traits/all_of.hpp
  ${prefix}/tite/type_traits/allocator_tag.hpp
  ${prefix}/tite/type_traits/invocable.hpp
  ${prefix}/tite/type_traits/is_cpo_invocable.hpp
  ${prefix}/tite/type_traits/is_specialization_of.hpp
  ${prefix}/tite/type_traits/is_type_erasable.hpp
  ${prefix}/tite/type_traits/remove_cvref.hpp
  ${prefix}/tite/type_traits/requires.hpp
  ${prefix}/tite/type_traits/tag_invoke.hpp
  ${prefix}/tite/type_traits/void_t.hpp

  # utlity
  ${prefix}/tite/utility/in_place.hpp
)

if( TITE_TESTING )
  add_subdirectory(tite/self.test)
  add_subdirectory(tite/type_traits.test)
  add_subdirectory(tite/overload.test)
  add_subdirectory(tite/gpu_allocator.test)
  add_subdirectory(tite/vtable.test)
  add_subdirectory(tite/any_unique.test)
endif()
