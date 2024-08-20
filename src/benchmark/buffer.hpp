#pragma once

#include <ipps.h>
#include <stdexcept>
#include <cmath>   // std::floor
#include <utility> // std::exchange
#include <string>  // std::to_string
#include <initializer_list>

namespace internal
{
    template <typename T>
    inline void copy(const T *src, T *dst, int size) {};

    template <>
    inline void copy<Ipp64f>(const Ipp64f *src, Ipp64f *dst, int size)
    {
        IppStatus status = ippsCopy_64f(src, dst, size);
        if (status != ippStsNoErr)
        {
            throw std::runtime_error("Unable to copy float64 buffer");
        }
    }

    template <>
    inline void copy<Ipp32f>(const Ipp32f *src, Ipp32f *dst, int size)
    {
        IppStatus status = ippsCopy_32f(src, dst, size);
        if (status != ippStsNoErr)
        {
            throw std::runtime_error("Unable to copy float32 buffer");
        }
    }

    template <>
    inline void copy<Ipp8u>(const Ipp8u *src, Ipp8u *dst, int size)
    {
        IppStatus status = ippsCopy_8u(src, dst, size);
        if (status != ippStsNoErr)
        {
            throw std::runtime_error("Unable to copy uint8 buffer");
        }
    }

    template <>
    inline void copy<Ipp32u>(const Ipp32u *src, Ipp32u *dst, int size)
    {
        IppStatus status = ippsCopy_8u(reinterpret_cast<const Ipp8u *>(src),
                                       reinterpret_cast<Ipp8u *>(dst),
                                       size * sizeof(Ipp32u));
        if (status != ippStsNoErr)
        {
            throw std::runtime_error("Unable to copy Ipp32u buffer");
        }
    }

    template <typename T>
    inline T *allocate(int num_elements) { return nullptr; }

    template <>
    inline Ipp64f *allocate<Ipp64f>(int num_elements)
    {
        if (num_elements <= 0)
        {
            return nullptr;
        }

        Ipp64f *p = ippsMalloc_64f(num_elements);
        if (p == nullptr)
        {
            throw std::runtime_error("Unable to allocate memory for float64 buffer");
        }
        return p;
    }

    template <>
    inline Ipp32f *allocate<Ipp32f>(int num_elements)
    {
        if (num_elements <= 0)
        {
            return nullptr;
        }

        Ipp32f *p = ippsMalloc_32f(num_elements);
        if (p == nullptr)
        {
            throw std::runtime_error("Unable to allocate memory for float32 buffer");
        }
        return p;
    }

    template <>
    inline Ipp8u *allocate<Ipp8u>(int num_elements)
    {
        if (num_elements <= 0)
        {
            return nullptr;
        }

        Ipp8u *p = ippsMalloc_8u(num_elements);
        if (p == nullptr)
        {
            throw std::runtime_error("Unable to allocate memory for uint8 buffer");
        }
        return p;
    }

    template <>
    inline Ipp32u *allocate<Ipp32u>(int num_elements)
    {
        if (num_elements <= 0)
        {
            return nullptr;
        }

        Ipp32u *p = reinterpret_cast<Ipp32u *>(ippsMalloc_8u(num_elements * sizeof(Ipp32u)));
        if (p == nullptr)
        {
            throw std::runtime_error("Unable to allocate memory for Ipp32u buffer");
        }
        return p;
    }

    inline void slope(int n, Ipp32f start, Ipp32f step, Ipp32f *dst)
    {
        IppStatus status = ippsVectorSlope_32f(dst, n, start, step);
        if (status != ippStsNoErr)
        {
            throw std::runtime_error("Unable to initialise vector from a range of values");
        }
    }

    inline void slope(int n, Ipp64f start, Ipp64f step, Ipp64f *dst)
    {
        IppStatus status = ippsVectorSlope_64f(dst, n, start, step);
        if (status != ippStsNoErr)
        {
            throw std::runtime_error("Unable to initialise vector from a range of values");
        }
    }

    inline void slope(int n, Ipp8u start, Ipp8u step, Ipp8u *dst)
    {
        IppStatus status = ippsVectorSlope_8u(dst, n, start, step);
        if (status != ippStsNoErr)
        {
            throw std::runtime_error("Unable to initialise vector from a range of values");
        }
    }

    inline void slope(int n, Ipp32u start, Ipp32u step, Ipp32u *dst)
    {
        for (int k = 0; k < n; ++k)
        {
            dst[k] = start + n * step;
        }
    }

    template <typename T>
    class buffer_container
    {
    private:
        int n{0};
        T *ptr{nullptr};

    public:
        buffer_container() {};
        buffer_container(int length)
            : n(length), ptr(allocate<T>(length))
        {
        }

        buffer_container(std::initializer_list<T> src)
            : n(src.size()), ptr(allocate<T>(n))
        {
            copy(src.begin(), ptr, n);
        }

        buffer_container(const T *src, int length)
            : n(length), ptr(allocate<T>(n))
        {
            copy(src, ptr, n);
        }

        buffer_container(const buffer_container<T> &other)
            : n(other.n), ptr(allocate<T>(other.n))
        {
            copy(other.ptr, ptr, n);
        }

        buffer_container(buffer_container<T> &&other)
            : n(other.n), ptr(other.ptr)
        {
            other.n = 0;
            other.ptr = nullptr;
        }

        buffer_container &operator=(const buffer_container<T> &other)
        {
            if (this == &other)
            {
                return *this;
            }

            if (ptr != nullptr)
            {
                ippsFree(ptr);
            }

            n = 0;
            ptr = nullptr;

            ptr = allocate<T>(other.n);
            n = other.n;

            copy(other.ptr, ptr, n);

            return *this;
        }

        buffer_container &operator=(buffer_container<T> &&other)
        {
            if (this == &other)
            {
                return *this;
            }

            if (ptr != nullptr)
            {
                ippsFree(ptr);
            }

            ptr = std::exchange(other.ptr, nullptr);
            n = std::exchange(other.n, 0);
            return *this;
        }

        buffer_container(T start, T end, T step)
            : n(int(std::floor((end - start) / step))), ptr(allocate<T>(n))
        {
            slope(n, start, step, ptr);
        }

        ~buffer_container()
        {
            if (ptr != nullptr)
            {
                ippsFree(ptr);
            }
        }

        int size() const { return n; }
        operator T *() const { return ptr; }
        T *data() const { return ptr; }
        bool is_empty() const { return ptr == nullptr; }
    };
}

typedef internal::buffer_container<Ipp32f> buffer;
typedef internal::buffer_container<Ipp32f> buffer32f;
typedef internal::buffer_container<Ipp64f> buffer64f;
typedef internal::buffer_container<Ipp8u> buffer8u;
typedef internal::buffer_container<Ipp32u> buffer32u;
