#ifndef MSR_EXCEPTION_OUT_OF_RANGE_HPP_INCLUDED
#define MSR_EXCEPTION_OUT_OF_RANGE_HPP_INCLUDED

#include <msr/exception.hpp>
#include <msr/utility.hpp>

namespace msr {
    
    class out_of_range: public exception {
        MSR_CLASS_TYPE_DEFINATIONS_WITH_BASE(out_of_range, exception);
        using exception::exception;
    };
    
}

#endif
