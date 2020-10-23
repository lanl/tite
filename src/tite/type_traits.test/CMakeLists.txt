add_library(type_traits.test OBJECT
  is_cpo_invocable.test.cpp
  all_of.test.cpp
  is_type_erasable.test.cpp)

target_link_libraries(type_traits.test PRIVATE
  tite::tite_testing)

target_sources(tite_test_main PRIVATE
  $<TARGET_OBJECTS:type_traits.test> )
