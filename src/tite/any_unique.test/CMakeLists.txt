add_library(any_unique.test OBJECT
  any_unique.test.cpp )

target_link_libraries(any_unique.test PRIVATE
  tite::tite_testing)

target_sources(tite_test_main PRIVATE
  $<TARGET_OBJECTS:any_unique.test> )
