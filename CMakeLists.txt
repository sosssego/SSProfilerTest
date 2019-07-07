cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # using Clang
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
  target_compile_options( frdDeepLearn PRIVATE -march=native)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # using Intel C++
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
  target_compile_options( frdDeepLearn PRIVATE /arch:AVX)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  # using AppleClang
endif()

project(SSprofilerTest)

enable_testing()
add_subdirectory(googletest)
add_subdirectory(SSprofiler)


add_executable(SSprofilerTest tests.cpp)

target_link_libraries(SSprofilerTest PUBLIC gtest_main SSprofiller )
add_test(NAME SSprofilerTest
		COMMAND SSprofilerTest)
