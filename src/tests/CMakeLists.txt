include(CTest)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.0 
)

FetchContent_MakeAvailable(Catch2)

set(BENCHMARKS_TEST_NAME test_benchmarks)

set(BENCHMARKS_TEST_HEADERS
)

set(BENCHMARKS_TEST_SOURCES
	test_main.cpp
  test_random.cpp
  test_simd.cpp
)

add_executable(${BENCHMARKS_TEST_NAME}
    ${BENCHMARKS_TEST_HEADERS}
    ${BENCHMARKS_TEST_SOURCES}
)

target_include_directories(${BENCHMARKS_TEST_NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/benchmark/
)

target_link_libraries(${BENCHMARKS_TEST_NAME}
    PRIVATE Catch2::Catch2WithMain
    PRIVATE IPP::ippcore
    PRIVATE IPP::ipps
    PRIVATE IPP::ippvm
    PRIVATE IPP::ippi
    PRIVATE IPP::ippcc    
)    

add_test(NAME ${BENCHMARKS_TEST_NAME}
         COMMAND  ${BENCHMARKS_TEST_NAME})