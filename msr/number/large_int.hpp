#ifndef MSR_LARGE_INT_HPP_INCLUDED
#define MSR_LARGE_INT_HPP_INCLUDED

#include <bitset>
#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <type_traits>
#include <vector>
#include <cmath>

namespace msr {
    class large_int {
    public:
        using self = large_int;
    private:
        using unit_t = uint8_t;
        using dual_t = uint16_t;
        using largest_t = unsigned long long;
    public:
        large_int();
        large_int(const self &another);
        large_int(self &&another);
        template <class T, class C = typename
        std::enable_if<std::is_integral<T>::value>::type>
        large_int(const T &num);
    public:
        self &operator=(const self &another);
        self &operator=(self &&another);
        friend bool operator==(const self &a, const self &b);
        friend bool operator!=(const self &a, const self &b);
        friend bool operator<(const self &a, const self &b);
        friend bool operator>(const self &a, const self &b);
        friend bool operator<=(const self &a, const self &b);
        friend bool operator>=(const self &a, const self &b);
        friend self operator+(const self &a, const self &b);
        self &operator+=(const self &another);
        self &operator++();
        self operator++(int);
        self operator-() const;
        friend self operator-(const self &a, const self &b);
        self &operator-=(const self &another);
        self &operator--();
        self operator--(int);
        template <class Char>
        friend std::basic_ostream<Char> &operator<<(std::basic_ostream<Char> &os, const self &n);
    public:
        bool signed_;
        std::vector<unit_t> num_;
    };
}

#include <msr/number/detail/large_int.ipp>

#endif
