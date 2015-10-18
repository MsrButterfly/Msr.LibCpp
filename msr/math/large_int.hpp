#ifndef MSR_MATH_LARGE_INT_HPP_INCLUDED
#define MSR_MATH_LARGE_INT_HPP_INCLUDED

#include <ios>
#include <limits>
#include <type_traits>
#include <vector>
#include <msr/exception.hpp>
#include <msr/math/absolute.hpp>
#include <msr/math/division.hpp>
#include <msr/memory.hpp>
#include <msr/utility.hpp>

namespace msr {

    class large_int {
        MSR_CLASS_TYPE_DEFINATIONS(large_int);
    public:
        large_int() noexcept;
        large_int(const self_type &another) noexcept;
        large_int(self_type &&another) noexcept;
        template <class T, class C = typename std::enable_if<std::is_integral<T>::value>::type>
        large_int(const T &num) noexcept;
    public:
        template <class T, class C = typename std::enable_if<std::is_integral<T>::value>::type>
        bool representable() const noexcept;
        template <class T, class C = typename std::enable_if<std::is_integral<T>::value>::type>
        T get() const throw(out_of_range);
    public:
        self_type operator~() const noexcept;
        self_type operator+() const noexcept;
        self_type operator-() const noexcept;
        self_type operator++(int) noexcept;
        self_type operator--(int) noexcept;
        self_type &operator=(const self_type &another) noexcept;
        self_type &operator=(self_type &&another) noexcept;
        self_type &operator++() noexcept;
        self_type &operator--() noexcept;
        self_type &operator<<=(const size_t &shift) noexcept;
        self_type &operator>>=(const size_t &shift) noexcept;
        self_type &operator&=(const self_type &another) noexcept;
        self_type &operator|=(const self_type &another) noexcept;
        self_type &operator^=(const self_type &another) noexcept;
        self_type &operator+=(const self_type &another) noexcept;
        self_type &operator-=(const self_type &another) noexcept;
        self_type &operator*=(const self_type &another) noexcept;
        self_type &operator/=(const self_type &another) throw(divide_by_zero);
        self_type &operator%=(const self_type &another) throw(divide_by_zero);
        friend bool operator==(const self_type &a, const self_type &b) noexcept;
        friend bool operator!=(const self_type &a, const self_type &b) noexcept;
        friend bool operator<(const self_type &a, const self_type &b) noexcept;
        friend bool operator>(const self_type &a, const self_type &b) noexcept;
        friend bool operator<=(const self_type &a, const self_type &b) noexcept;
        friend bool operator>=(const self_type &a, const self_type &b) noexcept;
        friend self_type operator<<(const self_type &value, const size_t &shift) noexcept;
        friend self_type operator>>(const self_type &value, const size_t &shift) noexcept;
        friend self_type operator&(const self_type &a, const self_type &b) noexcept;
        friend self_type operator|(const self_type &a, const self_type &b) noexcept;
        friend self_type operator^(const self_type &a, const self_type &b) noexcept;
        friend self_type operator+(const self_type &a, const self_type &b) noexcept;
        friend self_type operator-(const self_type &a, const self_type &b) noexcept;
        friend self_type operator*(const self_type &a, const self_type &b) noexcept;
        friend self_type operator/(const self_type &a, const self_type &b) throw(divide_by_zero);
        friend self_type operator%(const self_type &a, const self_type &b) throw(divide_by_zero);
        friend div_t<self_type> div(const self_type &a, const self_type &b) throw(divide_by_zero);
        explicit operator bool() const noexcept;
    public:
        friend self_type abs(const self_type &n) noexcept;
        template <class Char>
        friend std::basic_ostream<Char> &operator<<(std::basic_ostream<Char> &os, const self_type &n) noexcept;
        template <unsigned int Ary, class Char, class C>
        friend std::basic_ostream<Char> &output(std::basic_ostream<Char> &os, const self_type &n) noexcept;
    private:
        bool signed_;
        std::vector<byte> num_;
    };

}

#include <msr/math/detail/absolute.ipp>
#include <msr/math/detail/division.ipp>
#include <msr/math/detail/large_int.ipp>

#endif
