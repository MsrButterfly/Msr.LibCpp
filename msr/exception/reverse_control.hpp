#ifndef MSR_EXCEPTION_REVERSE_CONTROL_HPP_INCLUDED
#define MSR_EXCEPTION_REVERSE_CONTROL_HPP_INCLUDED

#include <msr/exception.hpp>
#include <msr/utility.hpp>

namespace msr {
    
    class reverse_control: public exception {
        MSR_CLASS_TYPE_DEFINATIONS_WITH_BASE(reverse_control, exception);
        using exception::exception;
    };
    
}

#endif
