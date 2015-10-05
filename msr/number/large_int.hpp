#ifndef MSR_NUMBER_LARGE_INT_HPP_INCLUDED
#define MSR_NUMBER_LARGE_INT_HPP_INCLUDED

#include <ios>
#include <limits>
#include <type_traits>
#include <vector>
#include <msr/number/absolute.hpp>
#include <msr/number/bit.hpp>
#include <msr/number/division.hpp>
#include <msr/number/large_int_divide_by_zero.hpp>

namespace msr {
    namespace detail {}
    class large_int {
    public:
        using self_type = large_int;
        using shift_type = std::size_t;
        using divide_by_zero = large_int_divide_by_zero;
    private:
        using unit_type = uint8_t;
        using dual_type = uint16_t;
        enum {
            unit_bits = type_bit_size<unit_type>::value,
#if defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 180021114
            unit_max = static_cast<unit_type>(-1)
#else
            unit_max = std::numeric_limits<unit_type>::max()
#endif
        };
    public:
        large_int() MSR_NOEXCEPT;
        large_int(const self_type &another) MSR_NOEXCEPT;
        large_int(self_type &&another) MSR_NOEXCEPT;
        template <class T, class C = typename
        std::enable_if<std::is_integral<T>::value>::type>
        large_int(const T &num) MSR_NOEXCEPT;
    public:
        self_type operator~() const MSR_NOEXCEPT;
        self_type operator+() const MSR_NOEXCEPT;
        self_type operator-() const MSR_NOEXCEPT;
        self_type operator++(int) MSR_NOEXCEPT;
        self_type operator--(int) MSR_NOEXCEPT;
        self_type &operator=(const self_type &another) MSR_NOEXCEPT;
        self_type &operator=(self_type &&another) MSR_NOEXCEPT;
        self_type &operator++() MSR_NOEXCEPT;
        self_type &operator--() MSR_NOEXCEPT;
        self_type &operator<<=(const shift_type &another) MSR_NOEXCEPT;
        self_type &operator>>=(const shift_type &another) MSR_NOEXCEPT;
        self_type &operator&=(const self_type &another) MSR_NOEXCEPT;
        self_type &operator|=(const self_type &another) MSR_NOEXCEPT;
        self_type &operator^=(const self_type &another) MSR_NOEXCEPT;
        self_type &operator+=(const self_type &another) MSR_NOEXCEPT;
        self_type &operator-=(const self_type &another) MSR_NOEXCEPT;
        self_type &operator*=(const self_type &another) MSR_NOEXCEPT;
        self_type &operator/=(const self_type &another) MSR_THROW(divide_by_zero);
        self_type &operator%=(const self_type &another) MSR_THROW(divide_by_zero);
        friend bool operator==(const self_type &a, const self_type &b) MSR_NOEXCEPT;
        friend bool operator!=(const self_type &a, const self_type &b) MSR_NOEXCEPT;
        friend bool operator<(const self_type &a, const self_type &b) MSR_NOEXCEPT;
        friend bool operator>(const self_type &a, const self_type &b) MSR_NOEXCEPT;
        friend bool operator<=(const self_type &a, const self_type &b) MSR_NOEXCEPT;
        friend bool operator>=(const self_type &a, const self_type &b) MSR_NOEXCEPT;
        friend self_type operator<<(const self_type &a, const shift_type &b) MSR_NOEXCEPT;
        friend self_type operator>>(const self_type &a, const shift_type &b) MSR_NOEXCEPT;
        friend self_type operator&(const self_type &a, const self_type &b) MSR_NOEXCEPT;
        friend self_type operator|(const self_type &a, const self_type &b) MSR_NOEXCEPT;
        friend self_type operator^(const self_type &a, const self_type &b) MSR_NOEXCEPT;
        friend self_type operator+(const self_type &a, const self_type &b) MSR_NOEXCEPT;
        friend self_type operator-(const self_type &a, const self_type &b) MSR_NOEXCEPT;
        friend self_type operator*(const self_type &a, const self_type &b) MSR_NOEXCEPT;
        friend self_type operator/(const self_type &a, const self_type &b) MSR_THROW(divide_by_zero);
        friend self_type operator%(const self_type &a, const self_type &b) MSR_THROW(divide_by_zero);
        friend div_t<self_type> div(const self_type &a, const self_type &b) MSR_THROW(divide_by_zero);
        explicit operator bool() const MSR_NOEXCEPT;
        friend self_type abs(const self_type &n) MSR_NOEXCEPT;
        template <class Char>
        friend std::basic_ostream<Char>
        &operator<<(std::basic_ostream<Char> &os, const self_type &n) MSR_NOEXCEPT;
        template <unsigned int Ary, class Char, class C>
        friend std::basic_ostream<Char>
        &output(std::basic_ostream<Char> &os, const self_type &n) MSR_NOEXCEPT;
    private:
        bool signed_;
        std::vector<unit_type> num_;
    };
}

#include <msr/number/detail/absolute.ipp>
#include <msr/number/detail/division.ipp>
#include <msr/number/detail/large_int.ipp>

#endif
