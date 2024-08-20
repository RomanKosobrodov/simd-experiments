cmake_minimum_required(VERSION 3.25.1)

project(simd_experiments VERSION 0.2 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW) 
endif()

set(BUILD_SHARED_LIBS OFF CACHE BOOL "do not build shared libraries" FORCE)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO  "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")

if (UNIX)
    set(IPP_INCLUDE_DIRECTORY_BENCHMARKS "${IPP_DIR}/../../../include/ipp/")
endif(UNIX)

set(IPP_SHARED OFF)
find_package(IPP REQUIRED)
if (NOT IPP_FOUND)
    message(STATUS "IPP was not found")
endif()


add_subdirectory(benchmark)

add_subdirectory(tests)
