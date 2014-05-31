#ifndef MSR_NUMBER_DIGIT_HPP_INCLUDED
#define MSR_NUMBER_DIGIT_HPP_INCLUDED

#include <cstdint>
#include <ios>
#include <type_traits>

namespace msr {
    template <unsigned int Ary>
    class digit {
    public:
        using self_type = digit;
        using value_type = std::uint16_t;
    public:
        static const auto ary = Ary;
    public:
        digit();
        template <class T, class C = typename
        std::enable_if<std::is_unsigned<T>::value>::type>
        digit(const T& n);
    public:
        const value_type &get() const;
        const value_type &carry() const;
        const value_type &borrow() const;
        self_type &operator=(const self_type &another);
        self_type &operator++();
        self_type &operator--();
        self_type operator++(int);
        self_type operator--(int);
        self_type &operator+=(const self_type &another);
        self_type &operator-=(const self_type &another);
        self_type &operator*=(const self_type &another);
        self_type &operator/=(const self_type &another);
        friend self_type operator+(const self_type &a, const self_type &b) { return self_type(a) += b; }
        friend self_type operator-(const self_type &a, const self_type &b) { return self_type(a) -= b; }
        friend self_type operator*(const self_type &a, const self_type &b) { return self_type(a) *= b; }
        friend self_type operator/(const self_type &a, const self_type &b) { return self_type(a) /= b; }
        template <class Char>
        friend std::basic_ostream<Char> &operator<<(std::basic_ostream<Char> &os, const self_type &n) {
            os << static_cast<unsigned int>(n.n_);
            return os;
        }
    private:
        value_type carry_;
        value_type borrow_;
        value_type n_;
    };
}

#include "detail/digit.ipp"

#endif
