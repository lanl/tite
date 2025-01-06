add_library(any_unique.test OBJECT
  any_unique.test.cpp )

target_link_libraries(any_unique.test PRIVATE
  tite::tite_testing)

if( TITE_ENABLE_GPU )
  add_library(any_unique.test.GPU OBJECT
    gpu_any_unique.test.cpp )

  set_source_files_properties(
    gpu_any_unique.test.cpp
  PROPERTIES
    LANGUAGE ${TITE_GPU_LANGUAGE} )

  set_target_properties(any_unique.test.GPU PROPERTIES
    LINKER_LANGUAGE ${TITE_GPU_LANGUAGE})

  target_link_libraries(any_unique.test.GPU PRIVATE
    tite::tite tite::tite_testing)
endif()

target_sources(tite_test_main PRIVATE
  $<TARGET_OBJECTS:any_unique.test>
  $<$<BOOL:${TITE_ENABLE_GPU}>:$<TARGET_OBJECTS:any_unique.test.GPU>>)
