#ifndef MSR_MATH_DIVISION_HPP_INCLUDED
#define MSR_MATH_DIVISION_HPP_INCLUDED

#include <msr/exception.hpp>

namespace msr {

    class large_int;
    
    template <class T>
    struct div_t {
        T quot;
        T rem;
    };
    
    inline div_t<int> div(const int &a, const int &b);
    inline div_t<long> div(const long &a, const long &b);
    inline div_t<long long> div(const long long &a, const long long &b);
    inline div_t<large_int> div(const large_int &a, const large_int &b) throw(divide_by_zero);
    
}

#endif
