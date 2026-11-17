add_library(model)
target_sources(model PRIVATE
  make_model_te.cpp
  model.hpp
  model_options.hpp
  model_te.hpp
)
set_source_files_properties(make_model_te.cpp PROPERTIES LANGUAGE ${TITE_GPU_LANGUAGE})

target_link_libraries(model PUBLIC tite::tite_testing)

set_target_properties(model PROPERTIES
  CUDA_SEPARABLE_COMPILATION ON
)

add_library(apply_model)
target_sources(apply_model PRIVATE
  apply_model.cpp
)
set_source_files_properties(apply_model.cpp PROPERTIES LANGUAGE ${TITE_GPU_LANGUAGE})

target_link_libraries(apply_model PUBLIC
  model
)

set_target_properties(apply_model PROPERTIES
  CUDA_SEPARABLE_COMPILATION ON
)

add_executable(tite_rdc_test_main tite_rdc_test_main.cpp)

target_link_libraries(tite_rdc_test_main PRIVATE apply_model)

foreach(arch IN LISTS CMAKE_CUDA_ARCHITECTURES)
  target_link_options(tite_rdc_test_main PRIVATE "$<$<CUDA_COMPILER_ID:Clang>:LINKER:--should-extract=sm_${arch}>")
endforeach()

add_test(NAME rdc_test COMMAND tite_rdc_test_main)
