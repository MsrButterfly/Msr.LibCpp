#ifndef MSR_NUMBER_LARGE_INT_HPP_INCLUDED
#define MSR_NUMBER_LARGE_INT_HPP_INCLUDED

#include <iostream>
#include <limits>
#include <type_traits>
#include <vector>
#include "bit.hpp"

namespace msr {
    class large_int {
    public:
        using self_type = large_int;
        using shift_type = std::size_t;
    private:
        using unit_type = uint8_t;
        using dual_type = uint16_t;
        enum {
            unit_bits = type_bit_size<unit_type>::value,
            unit_max = std::numeric_limits<unit_type>::max()
        };
    public:
        large_int();
        large_int(const self_type &another);
        large_int(self_type &&another);
        template <class T, class C = typename
        std::enable_if<std::is_integral<T>::value>::type>
        large_int(const T &num);
    public:
        self_type &operator=(const self_type &another);
        self_type &operator=(self_type &&another);
        friend bool operator==(const self_type &a, const self_type &b);
        friend bool operator!=(const self_type &a, const self_type &b);
        friend bool operator<(const self_type &a, const self_type &b);
        friend bool operator>(const self_type &a, const self_type &b);
        friend bool operator<=(const self_type &a, const self_type &b);
        friend bool operator>=(const self_type &a, const self_type &b);
        friend self_type operator<<(const self_type &a, const shift_type &b);
        friend self_type operator>>(const self_type &a, const shift_type &b);
        self_type &operator<<=(const shift_type &b);
        self_type &operator>>=(const shift_type &b);
        self_type operator+() const;
        friend self_type operator+(const self_type &a, const self_type &b);
        self_type &operator+=(const self_type &another);
        self_type &operator++();
        self_type operator++(int);
        self_type operator-() const;
        friend self_type operator-(const self_type &a, const self_type &b);
        self_type &operator-=(const self_type &another);
        self_type &operator--();
        self_type operator--(int);
        friend self_type operator*(const self_type &a, const self_type &b);
        self_type &operator*=(const self_type &another);
        explicit operator bool();
        template <class Char>
        friend std::basic_ostream<Char>
        &operator<<(std::basic_ostream<Char> &os, const self_type &n);
    private:
        template <unsigned int Ary, class Char>
        static std::basic_ostream<Char> &output(std::basic_ostream<Char> &os, const self_type &n);
    public:
        bool signed_;
        std::vector<unit_type> num_;
    };
}

#include "detail/large_int.ipp"

#endif
