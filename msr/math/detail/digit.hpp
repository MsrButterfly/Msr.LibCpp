#ifndef MSR_MATH_DETAIL_DIGIT_HPP_INCLUDED
#define MSR_MATH_DETAIL_DIGIT_HPP_INCLUDED

#include <cstdint>
#include <ios>
#include <type_traits>
#include <msr/utility.hpp>

namespace msr {
    namespace detail {
        
        template <unsigned int Ary>
        class digit {
            MSR_CLASS_TYPE_DEFINATIONS(digit);
        public:
            using value_type = uint8_t;
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
}

#include <msr/math/detail/digit.ipp>

#endif
