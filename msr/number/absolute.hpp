#ifndef MSR_NUMBER_ABSOLUTE_HPP_INCLUDED
#define MSR_NUMBER_ABSOLUTE_HPP_INCLUDED

#include "large_int.hpp"

namespace msr {
    inline int abs(const int &n);
    inline long abs(const long &n);
    inline long long abs(const long long &n);
    inline large_int abs(const large_int &n);
}

#endif
