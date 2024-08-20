#include <catch2/catch_test_macros.hpp>
#include "buffer.hpp"
#include "benchmark.hpp"
#include "random_numbers.hpp"
#include <algorithm>

namespace stdx = std::experimental;

TEST_CASE("native_simd can be written and read")
{
    constexpr int n = 16;
    buffer32f src(n);
    buffer32f result(n);
    std::vector<stdx::native_simd<float>> dst;

    generator g(37645);
    constexpr float low = 0.0f;
    constexpr float high = 1.0f;
    g.uniform(low, high, src.size(), src.data());

    copy_to(src, dst);
    copy_from(dst, result);

    for (size_t k = 0; k < src.size(); ++k)
    {
        REQUIRE(src[k] == result[k]);
    }
}