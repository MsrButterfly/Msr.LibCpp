#ifndef MSR_NUMBER_BIT_HPP_INCLUDED
#define MSR_NUMBER_BIT_HPP_INCLUDED

#include <numeric>
#include <limits>
#include <cstdint>

namespace msr {
    template <std::size_t n>
    struct number_bit_size {
        static const auto value = number_bit_size<(n >> 1)>::value + 1;
    };
    template <>
    struct number_bit_size<0> {
        static const std::size_t value = 0;
    };
    template <class T>
    struct type_bit_size {
        static const auto value = std::numeric_limits<T>::digits;
    };
}

#endif
