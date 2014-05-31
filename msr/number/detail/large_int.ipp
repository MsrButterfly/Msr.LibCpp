#ifndef MSR_NUMBER_DETAIL_LARGE_INT_IPP_INCLUDED
#define MSR_NUMBER_DETAIL_LARGE_INT_IPP_INCLUDED

#include <iomanip>
#include <sstream>
#include "../digit.hpp"

namespace msr {
    large_int::large_int()
    : signed_(false), num_{0} {}
    large_int::large_int(const self_type &another)
    : signed_(another.signed_), num_(another.num_) {}
    large_int::large_int(self_type &&another)
    : signed_(another.signed_), num_(std::move(another.num_)) {
        another.signed_ = false;
        another.num_ = {0};
    }
    template <class T, class C>
    large_int::large_int(const T &num)
    : signed_(std::is_signed<T>::value && num < 0) {
        unsigned long long n;
        n = signed_ ? -num : num;
        while (n > 0) {
            num_.push_back(static_cast<unit_type>(n));
            n >>= unit_bits;
        }
        if (num_.size() == 0) {
            num_.push_back(0);
        }
    }
    large_int &large_int::operator=(const self_type &another) {
        signed_ = another.signed_;
        num_ = another.num_;
        return *this;
    }
    large_int &large_int::operator=(self_type &&another) {
        signed_ = another.signed_;
        num_ = another.num_;
        another.signed_ = false;
        another.num_ = {0};
        return *this;
    }
    bool operator==(const large_int &a, const large_int &b) {
        if ((a.signed_ ^ b.signed_) || a.num_.size() != b.num_.size()) {
            return false;
        }
        for (std::size_t i = 0; i < a.num_.size(); i++) {
            if (a.num_[i] != b.num_[i]) {
                return false;
            }
        }
        return true;
    }
    bool operator!=(const large_int &a, const large_int &b) {
        if ((a.signed_ ^ b.signed_) || a.num_.size() != b.num_.size()) {
            return true;
        }
        for (std::size_t i = 0; i < a.num_.size(); i++) {
            if (a.num_[i] != b.num_[i]) {
                return true;
            }
        }
        return false;
    }
    bool operator<(const large_int &a, const large_int &b) {
        if (a.signed_ ^ b.signed_) {
            return a.signed_;
        }
        auto &signed_ = a.signed_;
        if (a.num_.size() != b.num_.size()) {
            return (a.num_.size() < b.num_.size()) ^ signed_;
        }
        for (auto i = a.num_.size(); i > 0; i--) {
            auto j = i - 1;
            if (a.num_[j] != b.num_[j]) {
                return (a.num_[j] < b.num_[j]) ^ signed_;
            }
        }
        return false;
    }
    bool operator>(const large_int &a, const large_int &b) {
        if (a.signed_ ^ b.signed_) {
            return b.signed_;
        }
        auto &signed_ = a.signed_;
        if (a.num_.size() != b.num_.size()) {
            return (a.num_.size() > b.num_.size()) ^ signed_;
        }
        for (auto i = a.num_.size(); i > 0; i--) {
            auto j = i - 1;
            if (a.num_[j] != b.num_[j]) {
                return (a.num_[j] > b.num_[j]) ^ signed_;
            }
        }
        return false;
    }
    bool operator<=(const large_int &a, const large_int &b) {
        if (a.signed_ ^ b.signed_) {
            return a.signed_;
        }
        auto &signed_ = a.signed_;
        if (a.num_.size() != b.num_.size()) {
            return (a.num_.size() < b.num_.size()) ^ signed_;
        }
        for (auto i = a.num_.size(); i > 0; i--) {
            auto j = i - 1;
            if (a.num_[j] != b.num_[j]) {
                return (a.num_[j] < b.num_[j]) ^ signed_;
            }
        }
        return true;
    }
    bool operator>=(const large_int &a, const large_int &b) {
        if (a.signed_ ^ b.signed_) {
            return a.signed_;
        }
        auto &signed_ = a.signed_;
        if (a.num_.size() != b.num_.size()) {
            return (a.num_.size() < b.num_.size()) ^ signed_;
        }
        for (auto i = a.num_.size(); i > 0; i--) {
            auto j = i - 1;
            if (a.num_[j] != b.num_[j]) {
                return (a.num_[j] < b.num_[j]) ^ signed_;
            }
        }
        return true;
    }
    large_int operator<<(const large_int &a, const large_int::shift_type &b) {
        auto c = a;
        return c <<= b;
    }
    large_int operator>>(const large_int &a, const large_int::shift_type &b) {
        auto c = a;
        return c >>= b;
    }
    large_int &large_int::operator<<=(const shift_type &another) {
        auto div = std::lldiv(another, unit_bits);
        using container = decltype(num_);
        container c(div.quot + num_.size());
        std::copy(begin(num_), end(num_), begin(c) + div.quot);
        num_ = std::move(c);
        if (div.rem > 0) {
            dual_type d = 0;
            for (auto &i : num_) {
                d = (static_cast<dual_type>(i) << div.rem) + d;
                i = static_cast<unit_type>(d);
                d >>= unit_bits;
            }
            if (d) {
                num_.push_back(static_cast<unit_type>(d));
            }
        }
        return *this;
    }
    large_int &large_int::operator>>=(const shift_type &another) {
        auto div = std::lldiv(another, unit_bits);
        if (num_.size() - div.quot <= 0) {
            return (*this = 0);
        }
        using container = decltype(num_);
        container c(num_.size() - div.quot);
        std::copy(begin(num_) + div.quot, end(num_), begin(c));
        num_ = std::move(c);
        if (div.rem > 0) {
            dual_type d = 0;
            for (auto i = num_.size(); i > 0; i--) {
                auto j = i - 1;
                d = ((static_cast<dual_type>(num_[j]) << unit_bits) >> div.rem) + d;
                num_[j] = static_cast<unit_type>(d >> unit_bits);
                d <<= unit_bits;
            }
            if (*num_.rbegin() == 0) {
                if (num_.size() > 1) {
                    num_.pop_back();
                } else {
                    signed_ = false;
                }
            }
            
        }
        return *this;
    }
    large_int large_int::operator+() const {
        return *this;
    }
    large_int operator+(const large_int &a, const large_int &b) {
        large_int c = a;
        c += b;
        return c;
    }
    large_int &large_int::operator+=(const self_type &another) {
        auto &a = num_;
        auto &b = another.num_;
        if (signed_ ^ another.signed_) {
            return *this -= -another;
        }
        std::size_t max_size = std::max(a.size(), b.size());
        while (a.size() < max_size) {
            a.push_back(0);
        }
        dual_type c = 0;
        for (std::size_t i = 0; i < max_size; i++) {
            dual_type ub = i < b.size() ? b[i] : 0;
            c = static_cast<dual_type>(a[i]) + ub + c;
            a[i] = static_cast<unit_type>(c);
            c >>= unit_bits;
        }
        if (c) {
            a.push_back(static_cast<unit_type>(c));
        }
        return *this;
    }
    large_int &large_int::operator++() {
        return operator+=(1);
    }
    large_int large_int::operator++(int) {
        auto a = *this;
        operator+=(1);
        return a;
    }
    large_int large_int::operator-() const {
        auto n = *this;
        if (n != 0) {
            n.signed_ = !n.signed_;
        }
        return n;
    }
    large_int operator-(const large_int &a, const large_int &b) {
        large_int c = a;
        c -= b;
        return c;
    }
    large_int &large_int::operator-=(const self_type &another) {
        auto &a = num_;
        auto &b = another.num_;
        if (signed_ ^ another.signed_) {
            return *this += -another;
        }
        std::size_t max_size = std::max(a.size(), b.size());
        while (a.size() < max_size) {
            a.push_back(0);
        }
        dual_type c = 0;
        for (std::size_t i = 0; i < max_size; i++) {
            c ^= unit_max;
            c++;
            c &= unit_max;
            dual_type ub = i < b.size() ? b[i] : 0;
            c = static_cast<dual_type>(a[i]) - ub - c;
            a[i] = static_cast<unit_type>(c);
            c >>= unit_bits;
        }
        if (c) {
            for (auto &i : a) {
                i ^= unit_max;
            }
            auto signed__ = signed_;
            signed_ = false;
            operator++();
            signed_ = !signed__;
        }
        while (*a.rbegin() == 0) {
            if (a.size() > 1) {
                a.pop_back();
            } else {
                signed_ = false;
                break;
            }
        }
        return *this;
    }
    large_int &large_int::operator--() {
        return operator-=(1);
    }
    large_int large_int::operator--(int) {
        auto n = *this;
        operator-=(1);
        return n;
    }
    large_int operator*(const large_int &a, const large_int &b) {
        auto c = a;
        return c *= b;
    }
    large_int &large_int::operator*=(const self_type &another) {
        auto a = num_;
        auto &b = another.num_;
        signed_ = signed_ ^ another.signed_;
        num_ = decltype(num_)(a.size() + b.size());
        for (std::size_t i = 0; i < b.size(); i++) {
            decltype(num_) part(a.size() + b.size());
            dual_type c = 0;
            for (std::size_t j = 0; j < a.size(); j++) {
                c = static_cast<dual_type>(a[j]) * static_cast<dual_type>(b[i]) + c;
                part[i + j] = static_cast<unit_type>(c);
                c >>= unit_bits;
            }
            if (c) {
                part[i + a.size()] = c;
            }
            c = 0;
            for (std::size_t j = 0; j < num_.size(); j++) {
                c = static_cast<dual_type>(num_[j]) + part[j] + c;
                num_[j] = static_cast<unit_type>(c);
                c >>= unit_bits;
            }
        }
        while (*num_.rbegin() == 0) {
            if (num_.size() > 1) {
                num_.pop_back();
            } else {
                signed_ = false;
                break;
            }
        }
        return *this;
    }
    large_int operator/(const large_int &a, const large_int &b) {
        auto c = a;
        return c /= b;
    }
    large_int &large_int::operator/=(const self_type &another) {
        if (!another) {
            throw std::string("Divided By Zero!!!");
        }
        signed_ = signed_ ^ another.signed_;
        self_type a = abs(), b = another.abs();
        self_type min = 0, max = a;
        self_type mean, product;
        while (min != max) {
            mean = std::move(((max + min) >> 1) + 1);
            product = std::move(mean * b);
            if (product > a) {
                max = std::move(mean - 1);
            } else {
                min = std::move(mean);
            }
        }
        num_ = std::move(min.num_);
        if (num_.size() == 1 && *num_.rbegin() == 0) {
            signed_ = false;
        }
        return *this;
    }
    large_int large_int::abs() const {
        auto a = *this;
        a.signed_ = false;
        return a;
    }
    large_int abs(const large_int &n) {
        return n.abs();
    }
    large_int::operator bool() const {
        return num_.size() != 1 || *num_.rbegin() != 0;
    }
    template <class Char>
    std::basic_ostream<Char> &operator<<(std::basic_ostream<Char> &os, const large_int &n) {
        const auto &flags = os.flags();
        if (flags & std::ios::oct) {
            return n.output<8>(os, n);
        } else if (flags & std::ios::dec) {
            return n.output<10>(os, n);
        } else if (flags & std::ios::hex) {
            return n.output<16>(os, n);
        }
        return os;
    }
    template <unsigned int Ary, class Char>
    std::basic_ostream<Char> &large_int::output(std::basic_ostream<Char> &os, const self_type &n) {
        if (n.signed_) {
            os << '-';
        }
        const std::size_t length = large_int::unit_bits * n.num_.size() / (number_bit_size<Ary>::value - 1) + 1;
        std::vector<digit<Ary>> sum(length, 0u);
        std::vector<digit<Ary>> pow(length, 0u);
        pow[0]++;
        std::size_t pow_size = 1;
        auto bit_of = [&](const std::size_t &i) {
            auto div = std::lldiv(i, large_int::unit_bits);
            return n.num_[div.quot] & (1ll << div.rem);
        };
        for (std::size_t i = 0; i < n.num_.size() * large_int::unit_bits; i++) {
            if (bit_of(i)) {
                sum[0] += pow[0];
                auto carry = sum[0].carry();
                auto last_carry = carry;
                for (std::size_t j = 1; j < pow_size; j++) {
                    sum[j] += last_carry;
                    carry = sum[j].carry();
                    sum[j] += pow[j];
                    last_carry = sum[j].carry() + carry;
                }
                sum[pow_size] += last_carry;
            }
            pow[0] *= 2u;
            auto carry = pow[0].carry();
            auto last_carry = carry;
            for (std::size_t j = 1; j < pow_size; j++) {
                pow[j] *= 2u;
                carry = pow[j].carry();
                pow[j] += last_carry;
                last_carry = pow[j].carry() + carry;
            }
            if (last_carry) {
                pow[pow_size++] = last_carry;
            }
        }
        auto i = pow_size - 1;
        for (; i > 0 && sum[i].get() == 0u; i--);
        i++;
        std::basic_stringstream<Char> ss;
        ss.flags(os.flags());
        for (; i > 0; i--) {
            auto j = i - 1;
            ss << sum[j];
        }
        os << ss.str();
        return os;
    }
}

#endif
