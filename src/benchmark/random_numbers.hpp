#pragma once

#include <random>

class generator
{
    std::random_device rd;
    std::mt19937 mt;

public:
    generator(size_t seed)
        : rd{}, mt(rd())
    {
        mt.seed(seed);
    }

    void uniform(float low, float high, size_t size, float *dst)
    {
        std::uniform_real_distribution<> d(low, high);
        for (size_t k = 0; k < size; ++k)
        {
            dst[k] = d(mt);
        }
    }
};