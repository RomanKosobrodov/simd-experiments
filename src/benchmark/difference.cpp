#include "benchmark.hpp"
#include "random_numbers.hpp"
#include "stats.hpp"
#include "util.hpp"
#include <ipps.h>
#include <ippcore.h>
#include <print>

int main(int argc, char *argv[])
{
    ippInit();

    constexpr int n = 1024;
    constexpr size_t cycles = 10000;
    constexpr size_t runs = 31;
    constexpr size_t warm_up = 3;

    buffer32f x(n);
    buffer32f y(n);
    buffer32f z_ref(n);
    buffer32f z_ipp(n);
    buffer32f z_simd(n);
    buffer32f d(n);

    statistics s1;
    statistics s2;

    initialise(n, x, y, z_ref);
    double t_loop = loop_benchmark(cycles, x, y, z_ref);

    initialise(n, x, y, z_ipp);
    double t_ipp = ipp_benchmark(cycles, x, y, z_ipp);

    initialise(n, x, y, z_simd);
    double t_simd = simd_benchmark(cycles, x, y, z_simd);

    ippsSub_32f(z_ipp, z_ref, d, d.size());
    s1 = stat(d);

    ippsSub_32f(z_simd, z_ref, d, d.size());
    s2 = stat(d);

    std::println("Loop vs. IPP:  min={}; max={}, mean={}, stddev={}", s1.min, s1.max, s1.mean, s1.stddev);
    std::println("Loop vs. SIMD: min={}; max={}, mean={}, stddev={}", s2.min, s2.max, s2.mean, s2.stddev);
}