#pragma once

#include "buffer.hpp"
#include "random_numbers.hpp"

#include <ipps.h>

inline void initialise(int size, buffer32f &x, buffer32f &y, buffer32f &z, size_t seed = 37645327)
{
    buffer32f xt(size);
    buffer32f yt(size);
    buffer32f zt(size);

    generator g(seed);
    constexpr float low = 0.0f;
    constexpr float high = 1.0f;
    g.uniform(low, high, size, xt.data());
    g.uniform(low, high, size, yt.data());
    ippsSet_32f(0.0f, zt.data(), zt.size());

    x = std::move(xt);
    y = std::move(yt);
    z = std::move(zt);
}