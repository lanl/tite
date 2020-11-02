set(prefix $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>$<INSTALL_INTERFACE:include/tite>)
target_sources(tite INTERFACE
  ${prefix}/tite.hpp

  ${prefix}/tite/self.hpp
  ${prefix}/tite/overload.hpp
  ${prefix}/tite/default_invoke_pointer_policy.hpp
  ${prefix}/tite/cuda_invoke_pointer_policy.hpp
  ${prefix}/tite/vtable.hpp
  ${prefix}/tite/any_unique.hpp

  # detail
  ${prefix}/tite/detail/vtable_invoke.hpp
  ${prefix}/tite/detail/vtable_entry.hpp
  ${prefix}/tite/detail/vtable.hpp
  ${prefix}/tite/detail/vtable_holder.hpp

  # type_traits
  ${prefix}/tite/type_traits.hpp

  ${prefix}/tite/type_traits/void_t.hpp
  ${prefix}/tite/type_traits/tag_invoke.hpp

  ${prefix}/tite/type_traits/invocable.hpp
  ${prefix}/tite/type_traits/is_cpo_invocable.hpp

  ${prefix}/tite/type_traits/requires.hpp)

if( TITE_TESTING )
  add_subdirectory(tite/self.test)
  add_subdirectory(tite/type_traits.test)
  add_subdirectory(tite/overload.test)
  add_subdirectory(tite/vtable.test)
  # add_subdirectory(tite/any_unique.test)
endif()
