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
    buffer32f z(n);

    statistics s1;
    statistics s2;
    statistics s3;

    std::println("\"Loop\", \"IPP\", \"SIMD\"");

    constexpr float tol1 = 5e-3f;
    constexpr float tol2 = 1e-16f;
    for (size_t r = 0; r < runs + warm_up; ++r)
    {
        initialise(n, x, y, z);
        double t_loop = loop_benchmark(cycles, x, y, z);
        s1 = stat(z);

        initialise(n, x, y, z);
        double t_ipp = ipp_benchmark(cycles, x, y, z);
        s2 = stat(z);

        initialise(n, x, y, z);
        double t_simd = simd_benchmark(cycles, x, y, z);
        s3 = stat(z);

        if (close(s1, s2, tol1) && close(s1, s3, tol2) && r > warm_up)
        {
            std::println("{}, {}, {}", t_loop, t_ipp, t_simd);
        }
    }
}