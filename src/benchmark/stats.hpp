#pragma once
#include "buffer.hpp"

struct statistics
{
    float min{0};
    float max{0};
    float mean{0};
    float stddev{0};
};

inline statistics stat(const buffer32f &x)
{
    statistics s;
    ippsMinMax_32f(x.data(), x.size(), &s.min, &s.max);
    ippsMeanStdDev_32f(x.data(), x.size(), &s.mean, &s.stddev, ippAlgHintAccurate);
    return s;
}

inline bool close(const statistics &x, const statistics &y, float tolerance)
{
    return std::abs(x.min - y.min) < tolerance &&
           std::abs(x.max - y.max) < tolerance &&
           std::abs(x.mean - y.mean) < tolerance &&
           std::abs(x.stddev - y.stddev) < tolerance;
}