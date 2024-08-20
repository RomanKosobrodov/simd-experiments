set(BENCHMARKS "benchmark")

set(BENCHMARKS_HEADER_FILES
    benchmark.hpp
    buffer.hpp
    random_numbers.hpp
    stats.hpp
    util.hpp
)

set(BENCHMARKS_SOURCE_FILES
    main.cpp
)

add_executable(${BENCHMARKS} 
    ${BENCHMARKS_HEADER_FILES}
    ${BENCHMARKS_SOURCE_FILES}
)

target_include_directories(${BENCHMARKS} 
    PRIVATE ${IPP_INCLUDE_DIRECTORY_BENCHMARKS}
)

target_link_libraries(${BENCHMARKS}
    PRIVATE IPP::ippcore
    PRIVATE IPP::ipps
)

target_compile_definitions(${BENCHMARKS} PRIVATE CMAKE_EXPORT_COMPILE_COMMANDS=1)


set(DIFFERENCE "difference")

add_executable(${DIFFERENCE} 
    difference.cpp
)

target_include_directories(${DIFFERENCE} 
    PRIVATE ${IPP_INCLUDE_DIRECTORY_BENCHMARKS}
)

target_link_libraries(${DIFFERENCE}
    PRIVATE IPP::ippcore
    PRIVATE IPP::ipps
)


