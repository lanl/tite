set(prefix $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>$<INSTALL_INTERFACE:include/tite>)
target_sources(tite INTERFACE
  ${prefix}/tite.hpp

  ${prefix}/tite/self.hpp

  # type_traits
  ${prefix}/tite/type_traits.hpp

  ${prefix}/tite/type_traits/void_t.hpp

  ${prefix}/tite/type_traits/tag_invoke.hpp)

if( TITE_TESTING )
  add_subdirectory(tite/self.test)
endif()
