#ifndef MSR_EXCEPTION_TYPE_MISMATCH_HPP_INCLUDED
#define MSR_EXCEPTION_TYPE_MISMATCH_HPP_INCLUDED

#include <msr/exception.hpp>
#include <msr/utility.hpp>

namespace msr {
    
    class type_mismatch: public exception {
        MSR_CLASS_TYPE_DEFINATIONS_WITH_BASE(type_mismatch, exception);
        using exception::exception;
    };
    
}

#endif
