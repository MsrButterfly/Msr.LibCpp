#ifndef MSR_NUMBER_DETAIL_DIVISION_IPP_INCLUDED
#define MSR_NUMBER_DETAIL_DIVISION_IPP_INCLUDED

#include <cstdlib>

namespace msr {
    inline div_t<int> div(const int &a, const int &b) {
        auto r = std::div(a, b);
        return {r.quot, r.rem};
    }
    inline div_t<long> div(const long &a, const long &b) {
        auto r = std::ldiv(a, b);
        return {r.quot, r.rem};
    }
    inline div_t<long long> div(const long long &a, const long long &b) {
        auto r = std::lldiv(a, b);
        return {r.quot, r.rem};
    }
    inline div_t<large_int> div(const large_int &a, const large_int &b) MSR_THROW(large_int_divide_by_zero) {
        auto d = a / b;
        return {d, a - b * d};
    }
}

#endif
