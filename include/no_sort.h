/**
 * @file no_sort.h
 * Masks std sorting algorithms with error messages to prevent students from
 *  using the STL's sorting functions.
 * @author Sean Massung
 * @date Summer 2012
 */

#ifndef NO_SORT_H_
#define NO_SORT_H_

#include <algorithm>
#include <iostream>
#include <vector>

// included specifically for MP6
#include "point.h"

#define NOPE(X)                                                                \
    template <>                                                                \
    inline void sort(std::vector<X>::iterator, std::vector<X>::iterator)       \
    {                                                                          \
        std::cerr << "ERROR: std::sort not permitted" << std::endl;            \
    }                                                                          \
    template <class C>                                                         \
    inline void sort(std::vector<X>::iterator, std::vector<X>::iterator, C)    \
    {                                                                          \
        std::cerr << "ERROR: std::sort not permitted" << std::endl;            \
    }                                                                          \
    template <>                                                                \
    inline void sort_heap(std::vector<X>::iterator, std::vector<X>::iterator)  \
    {                                                                          \
        std::cerr << "ERROR: std::sort_heap not permitted" << std::endl;       \
    }                                                                          \
    template <class C>                                                         \
    inline void sort_heap(std::vector<X>::iterator, std::vector<X>::iterator,  \
                          C)                                                   \
    {                                                                          \
        std::cerr << "ERROR: std::sort_heap not permitted" << std::endl;       \
    }                                                                          \
    template <>                                                                \
    inline void stable_sort(std::vector<X>::iterator,                          \
                            std::vector<X>::iterator)                          \
    {                                                                          \
        std::cerr << "ERROR: std::stable_sort not permitted" << std::endl;     \
    }                                                                          \
    template <class C>                                                         \
    inline void stable_sort(std::vector<X>::iterator,                          \
                            std::vector<X>::iterator, C)                       \
    {                                                                          \
        std::cerr << "ERROR: std::stable_sort not permitted" << std::endl;     \
    }                                                                          \
    template <class C>                                                         \
    inline std::vector<X>::iterator partition(std::vector<X>::iterator left,   \
                                              std::vector<X>::iterator, C)     \
    {                                                                          \
        std::cerr << "ERROR: std::partition not permitted" << std::endl;       \
        return left;                                                           \
    }                                                                          \
    template <class C>                                                         \
    inline std::vector<X>::iterator stable_partition(                          \
        std::vector<X>::iterator left, std::vector<X>::iterator, C)            \
    {                                                                          \
        std::cerr << "ERROR: std::stable_partition not permitted"              \
                  << std::endl;                                                \
        return left;                                                           \
    }                                                                          \
    template <class C>                                                         \
    inline void nth_element(std::vector<X>::iterator,                          \
                            std::vector<X>::iterator)                          \
    {                                                                          \
        std::cerr << "ERROR: std::nth_element not permitted" << std::endl;     \
    }                                                                          \
    template <class C>                                                         \
    inline void nth_element(std::vector<X>::iterator,                          \
                            std::vector<X>::iterator, C)                       \
    {                                                                          \
        std::cerr << "ERROR: std::nth_element not permitted" << std::endl;     \
    }                                                                          \
    template <class C>                                                         \
    inline void partial_sort(std::vector<X>::iterator,                         \
                             std::vector<X>::iterator)                         \
    {                                                                          \
        std::cerr << "ERROR: std::partial_sort not permitted" << std::endl;    \
    }                                                                          \
    template <class C>                                                         \
    inline void partial_sort(std::vector<X>::iterator,                         \
                             std::vector<X>::iterator, C)                      \
    {                                                                          \
        std::cerr << "ERROR: std::partial_sort not permitted" << std::endl;    \
    }

namespace std
{
NOPE(int)
NOPE(unsigned int)
NOPE(bool)
NOPE(char)
NOPE(unsigned char)
NOPE(signed char)
NOPE(double)
NOPE(long double)
NOPE(long)
NOPE(long long)
NOPE(unsigned long)
NOPE(unsigned long long)
NOPE(float)
NOPE(short)
NOPE(unsigned short)
NOPE(point<1>)
NOPE(point<2>)
NOPE(point<3>)
NOPE(point<4>)
NOPE(point<5>)
}

#endif
