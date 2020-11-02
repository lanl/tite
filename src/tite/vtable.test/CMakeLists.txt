add_library(vtable.test.CXX OBJECT
  vtable_entry.test.cpp
  vtable.test.cpp)
  # vtable_holder.test.cpp)

set_property(TARGET vtable.test.CXX PROPERTY TITE_DIRECT_VTABLE_SIZE_MAX 2)

target_link_libraries(vtable.test.CXX PRIVATE
  tite::tite_testing)

if( TITE_ENABLE_CUDA )
  add_library(vtable.test.CUDA OBJECT
    vtable_entry.test.cu
    vtable.test.cu )
  target_link_libraries(vtable.test.CUDA PRIVATE
    tite::tite)
  target_compile_options(vtable.test.CUDA PRIVATE
    -expt-relaxed-constexpr)
endif()

target_sources(tite_test_main PRIVATE
  $<TARGET_OBJECTS:vtable.test.CXX>
  $<$<BOOL:${TITE_ENABLE_CUDA}>:$<TARGET_OBJECTS:vtable.test.CUDA>>)
