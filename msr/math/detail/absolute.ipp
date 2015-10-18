#ifndef MSR_MATH_DETAIL_ABSOLUTE_IPP_INCLUDED
#define MSR_MATH_DETAIL_ABSOLUTE_IPP_INCLUDED

#include <cstdlib>

namespace msr {

    inline int abs(const int &n) {
        return std::abs(n);
    }
    
    inline long abs(const long &n) {
        return std::abs(n);
    }
    
    inline long long abs(const long long &n) {
        return std::abs(n);
    }
    
    inline large_int abs(const large_int &n) noexcept {
        auto m = n;
        m.signed_ = false;
        return m;
    }
    
}

#endif
