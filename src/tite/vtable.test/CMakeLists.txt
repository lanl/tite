add_library(vtable.test.CXX OBJECT
  vtable_entry.test.cpp
  vtable.test.cpp
  vtable_holder.test.cpp)

set_property(TARGET vtable.test.CXX PROPERTY TITE_DIRECT_VTABLE_SIZE_MAX 2)

target_link_libraries(vtable.test.CXX PRIVATE
  tite::tite_testing)

if( TITE_ENABLE_GPU )
  add_library(vtable.test.GPU OBJECT
    gpu_vtable_entry.test.cpp
    gpu_vtable.test.cpp
    gpu_vtable_holder.test.cpp )

  set_source_files_properties(
    gpu_vtable_entry.test.cpp
    gpu_vtable.test.cpp
    gpu_vtable_holder.test.cpp
  PROPERTIES
    LANGUAGE ${TITE_GPU_LANGUAGE} )

  set_target_properties(vtable.test.GPU PROPERTIES
    LINKER_LANGUAGE ${TITE_GPU_LANGUAGE})

  target_link_libraries(vtable.test.GPU PRIVATE
    tite::tite)
endif()

target_sources(tite_test_main PRIVATE
  $<TARGET_OBJECTS:vtable.test.CXX>
  $<$<BOOL:${TITE_ENABLE_CUDA}>:$<TARGET_OBJECTS:vtable.test.GPU>>)
