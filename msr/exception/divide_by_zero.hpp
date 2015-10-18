#ifndef MSR_EXCEPTION_DIVIDE_BY_ZERO_HPP_INCLUDED
#define MSR_EXCEPTION_DIVIDE_BY_ZERO_HPP_INCLUDED

#include <msr/exception.hpp>
#include <msr/utility.hpp>

namespace msr {
    
    class divide_by_zero: public exception {
        MSR_CLASS_TYPE_DEFINATIONS_WITH_BASE(divide_by_zero, exception);
        using exception::exception;
    };
    
}

#endif
