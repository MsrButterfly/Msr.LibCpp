#ifndef MSR_NUMBER_LARGE_INT_DIVIDED_BY_ZERO_HPP_INCLUDED
#define MSR_NUMBER_LARGE_INT_DIVIDED_BY_ZERO_HPP_INCLUDED

#include <msr/number/divide_by_zero.hpp>

namespace msr {
    class large_int_divide_by_zero: public divide_by_zero {
    public:
        using self_type = large_int_divide_by_zero;
        using base_type = divide_by_zero;
    public:
        large_int_divide_by_zero()
        : base_type(0x01, "msr::large_int::operator/=") {}
    };
}

#endif
