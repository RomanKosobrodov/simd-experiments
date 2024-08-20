#include <catch2/catch_test_macros.hpp>
#include "buffer.hpp"
#include "random_numbers.hpp"
#include <algorithm>

TEST_CASE("random numbers are generated")
{
    constexpr int n = 127;
    buffer32f x(n);
    generator g(284635);

    SECTION("within the expected range")
    {
        constexpr float low = 0.0f;
        constexpr float high = 1.0f;
        g.uniform(low, high, x.size(), x);

        auto ptr_min = std::min_element(x.data(), x.data() + x.size());
        auto ptr_max = std::max_element(x.data(), x.data() + x.size());

        REQUIRE(*ptr_min < *ptr_max);
        REQUIRE(*ptr_min >= low);
        REQUIRE(*ptr_max <= high);
    }
}