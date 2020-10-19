add_library(self.test OBJECT
  self.test.cpp )

target_link_libraries(self.test PRIVATE
  tite::tite_testing)

target_sources(tite_test_main PRIVATE
  $<TARGET_OBJECTS:self.test> )
