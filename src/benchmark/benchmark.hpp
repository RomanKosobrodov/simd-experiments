#include <ipps.h>
#include <experimental/simd>
#include <chrono>
#include <vector>
#include "buffer.hpp"

constexpr float alpha = 0.995f;
constexpr float beta = 0.999f;

namespace stdx = std::experimental;

inline void check_size(const buffer32f &x, const buffer32f &y, const buffer32f z)
{
    if (x.size() != y.size() || x.size() != z.size())
    {
        throw std::runtime_error("wrong buffer size");
    }
}

inline void copy_to(const buffer32f &src, std::vector<stdx::native_simd<float>> &dst)
{
    const size_t w = stdx::native_simd<float>::size();
    const size_t num = src.size() / w;
    dst.resize(num);
    auto p = src.data();
    for (size_t k = 0; k < num; ++k)
    {
        dst[k].copy_from(p, stdx::vector_aligned);
        p += w;
    }
}

inline void copy_from(const std::vector<stdx::native_simd<float>> &src, buffer32f &dst)
{
    const size_t w = stdx::native_simd<float>::size();
    const size_t num = src.size() * w;
    if (dst.size() != num)
    {
        throw std::runtime_error("buffer sizes do not match");
    }

    auto p = dst.data();
    for (size_t k = 0; k < src.size(); ++k)
    {
        src[k].copy_to(p, stdx::vector_aligned);
        p += w;
    }
}

inline double loop_benchmark(size_t cycles, buffer32f &x, buffer32f &y, buffer32f &z)
{
    check_size(x, y, z);

    const auto t0 = std::chrono::high_resolution_clock::now();
    for (size_t k = 0; k < cycles; ++k)
    {
        for (size_t p = 0; p < x.size(); ++p)
        {
            x[p] *= alpha;
            y[p] *= beta;
            z[p] += x[p] * x[p] + y[p] * y[p];
        }
    }
    const auto dt = std::chrono::high_resolution_clock::now() - t0;
    return std::chrono::duration_cast<std::chrono::microseconds>(dt).count();
}

inline double simd_benchmark(size_t cycles, buffer32f &x, buffer32f &y, buffer32f &z)
{
    check_size(x, y, z);

    const size_t num = x.size() / stdx::native_simd<float>::size();
    std::vector<stdx::native_simd<float>> xs(num);
    std::vector<stdx::native_simd<float>> ys(num);
    std::vector<stdx::native_simd<float>> zs(num);

    copy_to(x, xs);
    copy_to(y, ys);
    copy_to(z, zs);

    const auto t0 = std::chrono::high_resolution_clock::now();
    for (size_t k = 0; k < cycles; ++k)
    {
        for (size_t p = 0; p < xs.size(); ++p)
        {
            xs[p] *= alpha;
            ys[p] *= beta;
            zs[p] += xs[p] * xs[p] + ys[p] * ys[p];
        }
    }
    const auto dt = std::chrono::high_resolution_clock::now() - t0;

    copy_from(xs, x);
    copy_from(ys, y);
    copy_from(zs, z);

    return std::chrono::duration_cast<std::chrono::microseconds>(dt).count();
}

inline double ipp_benchmark(size_t cycles, buffer32f &x, buffer32f &y, buffer32f &z)
{
    check_size(x, y, z);

    const auto t0 = std::chrono::high_resolution_clock::now();
    for (size_t k = 0; k < cycles; ++k)
    {
        ippsMulC_32f_I(alpha, x.data(), x.size());
        ippsMulC_32f_I(beta, y.data(), y.size());
        ippsAddProduct_32f(x.data(), x.data(), z.data(), x.size());
        ippsAddProduct_32f(y.data(), y.data(), z.data(), y.size());
    }
    const auto dt = std::chrono::high_resolution_clock::now() - t0;
    return std::chrono::duration_cast<std::chrono::microseconds>(dt).count();
}
