if( TITE_ENABLE_GPU )
  add_library(host_held_gpu_allocator.test.GPU OBJECT
    host_held_gpu_allocator.test.cpp )

  set_source_files_properties(
    host_held_gpu_allocator.test.cpp
  PROPERTIES
    LANGUAGE ${TITE_GPU_LANGUAGE} )

  set_target_properties(host_held_gpu_allocator.test.GPU PROPERTIES
    LINKER_LANGUAGE ${TITE_GPU_LANGUAGE})

  target_link_libraries(host_held_gpu_allocator.test.GPU PRIVATE
    tite::tite tite::tite_testing)
endif()

target_sources(tite_test_main PRIVATE
  $<$<BOOL:${TITE_ENABLE_GPU}>:$<TARGET_OBJECTS:host_held_gpu_allocator.test.GPU>>)
