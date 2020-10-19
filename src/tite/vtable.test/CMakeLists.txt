add_library(vtable.test OBJECT
  vtable_entry.test.cpp )

target_link_libraries(vtable.test PRIVATE
  tite::tite_testing)

target_sources(tite_test_main PRIVATE
  $<TARGET_OBJECTS:vtable.test> )
