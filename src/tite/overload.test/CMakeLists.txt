add_library(overload.test OBJECT
  overload.test.cpp )

target_link_libraries(overload.test PRIVATE
  tite::tite_testing)

target_sources(tite_test_main PRIVATE
  $<TARGET_OBJECTS:overload.test> )
