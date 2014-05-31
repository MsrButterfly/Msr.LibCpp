#ifndef MSR_NUMBER_EXCEPTION_HPP_INCLUDED
#define MSR_NUMBER_EXCEPTION_HPP_INCLUDED

#include <cstdint>
#include <msr/exception.hpp>

namespace msr {
    class number_exception : public exception {
    public:
        using self_type = number_exception;
        using base_type = exception;
        using id_t = std::uint16_t;
    public:
        number_exception(const id_t &id, const char *module, const char *description = "(undefined)")
        : base_type(0x00010000 + id, module, description) {}
    };
    
}

#endif
