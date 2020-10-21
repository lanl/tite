cmake_minimum_required(VERSION 3.14.6)

set(tite_testing_default TRUE)
if( DEFINED PROJECT_NAME )
  set(tite_testing_default FALSE)
endif()

project(tite
  VERSION 1.0.0
  LANGUAGES CXX) # CUDA)

include(CTest)

include(CMakeDependentOption)

CMAKE_DEPENDENT_OPTION(TITE_TESTING
  "Build tite tests" ${tite_testing_default}
  "BUILD_TESTING" OFF)

add_library(tite INTERFACE)
add_library(tite::tite ALIAS tite)

target_compile_features(tite INTERFACE cxx_std_14)

target_include_directories(tite INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>
  $<INSTALL_INTERFACE:include>)

if( TITE_TESTING )
  find_package(Catch2 REQUIRED)

  set(CMAKE_CXX_EXTENSIONS OFF)

  add_library(tite_testing INTERFACE)
  add_library(tite::tite_testing ALIAS tite_testing)

  target_link_libraries(tite_testing INTERFACE
    Catch2::Catch2
    tite::tite)

   target_compile_options(tite_testing INTERFACE
    $<$<CXX_COMPILER_ID:GNU>:-Wall;-Werror;-Wno-sign-compare>
    $<$<NOT:$<BOOL:${WIN32}>>:$<$<CXX_COMPILER_ID:Clang>:-Wall;-Werror>>
    $<$<BOOL:${WIN32}>:$<$<CXX_COMPILER_ID:Clang>:-Wall;-Wno-c++98-compat;-Wno-c++98-compat-pedantic>>
    $<$<CXX_COMPILER_ID:AppleClang>:-Wall;-Werror >
   )

  add_executable(tite_test_main
    testing/catch_main.cpp )
  target_link_libraries(tite_test_main PRIVATE
    Catch2::Catch2 )

  add_subdirectory(src)

  include(Catch)
  catch_discover_tests(tite_test_main)
endif()

install(DIRECTORY src/
  DESTINATION include
  FILES_MATCHING PATTERN *.hpp)

install(TARGETS tite EXPORT titeTargets
  INCLUDES DESTINATION include)

install(EXPORT titeTargets
  FILE titeTargets.cmake
  NAMESPACE tite::
  DESTINATION share/cmake/tite)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(titeConfigVersion.cmake
  VERSION ${tite_VERSION}
  COMPATIBILITY SameMajorVersion)

install(FILES
  cmake/titeConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/titeConfigVersion.cmake
  DESTINATION share/cmake/tite)
