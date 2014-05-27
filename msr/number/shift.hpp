#ifndef MSR_NUMBER_SHIFT_HPP_INCLUDED
#define MSR_NUMBER_SHIFT_HPP_INCLUDED

#include <cstdint>

namespace msr {
    template <std::size_t n, std::size_t m>
    struct logical_shift_left {
        enum { value = n << m };
    };
    template <std::size_t n, std::size_t m>
    struct logical_shift_right {
        enum { value = n >> m };
    };
}

#endif
