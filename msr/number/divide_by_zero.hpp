#ifndef MSR_NUMBER_DIVIDE_BY_ZERO_HPP_INCLUDED
#define MSR_NUMBER_DIVIDE_BY_ZERO_HPP_INCLUDED

#include "exception.hpp"

namespace msr {
    class divide_by_zero: public number_exception {
    public:
        using self_type = divide_by_zero;
        using base_type = number_exception;
        using id_t = std::uint8_t;
    public:
        divide_by_zero(const id_t &id, const char *module)
        : base_type(0x0100 + id, module, "Divisor cannot be zero.") {}
    };
}

#endif
