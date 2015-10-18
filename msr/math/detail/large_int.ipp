#ifndef MSR_MATH_DETAIL_LARGE_INT_IPP_INCLUDED
#define MSR_MATH_DETAIL_LARGE_INT_IPP_INCLUDED

#include <algorithm>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <utility>
#include <msr/exception.hpp>
#include <msr/math/detail/digit.hpp>
#include <msr/memory.hpp>
#include <msr/utility.hpp>

namespace msr {

    large_int::large_int() noexcept
    : signed_(false), num_(0, 1) {}

    large_int::large_int(const self_type &another) noexcept
    : signed_(another.signed_), num_(another.num_) {}

    large_int::large_int(self_type &&another) noexcept
    : signed_(another.signed_), num_(std::move(another.num_)) {
        another.signed_ = false;
        another.num_ = {0};
    }

    template <class T, class C>
    large_int::large_int(const T &num) noexcept
    : signed_(std::is_signed<T>::value && num < 0) {
        uintmax_t n;
        n = signed_ ? -(intmax_t)num : num;
        while (n > 0) {
            num_.push_back(static_cast<byte>(n));
            n >>= MSR_BITS_PER_BYTE;
        }
        if (num_.size() == 0) {
            num_.push_back(0);
        }
    }

    template <class T, class C>
    bool large_int::representable() const noexcept {
        return *this >= std::numeric_limits<T>::min() && *this <= std::numeric_limits<T>::max();
    }

    template <class T, class C>
    T large_int::get() const throw(out_of_range) {
        using namespace std;
        if (!representable<T>()) {
            throw out_of_range(MSR_FUNCTION_SIGNATURE, "");
        }
        union {
            T value = 0;
            byte bytes[sizeof(T)];
        } ret;
        if (current_endian() == endian::little_endian) {
            memcpy(&ret, num_.data(), num_.size());
        } else if (current_endian() == endian::big_endian) {
            auto it = rbegin(ret.bytes);
#if defined(_MSC_VER) && _SECURE_SCL == 1
            // Tricking the Secure SCL.
            // reverse_iterator<byte *> is supported by the implementation,
            // but not the specification (raw pointer).
            // https://msdn.microsoft.com/en-us/library/aa985928.aspx
            auto dest = stdext::make_unchecked_array_iterator(it);
#else
            auto dest = it;
#endif
            copy(begin(num_), end(num_), dest);
        }
        if (signed_) {
            ret.value = -(intmax_t)ret.value;
            // ok: with T = int8_t, *this = -128
            // -(-128) = (~0x80) + 1 = 0x7F + 1 = 0x80 = -128
            // http://stackoverflow.com/questions/17469804/what-is-128-for-signed-single-byte-char-in-c
        }
        return ret.value;
    }

    large_int large_int::operator~() const noexcept {
        auto n = *this;
        size_t i;
        for (i = 0; i < n.num_.size() - 1; ++i) {
            n.num_[i] ^= byte_all_one;
        }
        byte h = n.num_[i];
        byte u;
        for (u = 0; h; h >>= 1) {
            u <<= 1;
            ++u;
        }
        n.num_[i] ^= u;
        decltype(num_)::reverse_iterator p;
        for (p = n.num_.rbegin(); p != n.num_.rend() && *p == 0; ++p);
        if (p == n.num_.rend()) {
            n.signed_ = false;
            n.num_ = {0};
        } else {
            n.num_.resize(n.num_.rend() - p);
        }
        return std::move(n);
    }

    large_int large_int::operator+() const noexcept {
        return *this;
    }

    large_int large_int::operator-() const noexcept {
        auto n = *this;
        if (n) {
            n.signed_ = !n.signed_;
        }
        return std::move(n);
    }

    large_int large_int::operator++(int) noexcept {
        auto n = *this;
        ++*this;
        *this += 1;
        return std::move(n);
    }

    large_int large_int::operator--(int) noexcept {
        auto n = *this;
        *this -= 1;
        return std::move(n);
    }

    large_int &large_int::operator++() noexcept {
        return *this += 1;
    }

    large_int &large_int::operator--() noexcept {
        return *this += 1;
    }

    large_int &large_int::operator=(const self_type &another) noexcept {
        signed_ = another.signed_;
        num_ = another.num_;
        return *this;
    }
    large_int &large_int::operator=(self_type &&another) noexcept {
        signed_ = another.signed_;
        num_ = another.num_;
        another.signed_ = false;
        another.num_ = {0};
        return *this;
    }

    large_int &large_int::operator<<=(const size_t &another) noexcept {
        auto div = std::lldiv(another, MSR_BITS_PER_BYTE);
        decltype(num_) c(static_cast<size_t>(div.quot) + num_.size());
        std::copy(begin(num_), end(num_), begin(c) + static_cast<size_t>(div.quot));
        num_ = std::move(c);
        if (div.rem > 0) {
            dual_byte d = 0;
            for (auto &i : num_) {
                d = (static_cast<dual_byte>(i) << div.rem) + d;
                i = static_cast<byte>(d);
                d >>= MSR_BITS_PER_BYTE;
            }
            if (d) {
                num_.push_back(static_cast<byte>(d));
            }
        }
        return *this;
    }

    large_int &large_int::operator>>=(const size_t &another) noexcept {
        auto div = std::lldiv(another, MSR_BITS_PER_BYTE);
        if (num_.size() - div.quot <= 0) {
            return (*this = 0);
        }
        decltype(num_) c(num_.size() - static_cast<size_t>(div.quot));
        std::copy(begin(num_) + static_cast<size_t>(div.quot), end(num_), begin(c));
        num_ = std::move(c);
        if (div.rem > 0) {
            dual_byte d = 0;
            for (auto i = num_.size(); i > 0; i--) {
                auto j = i - 1;
                d = ((static_cast<dual_byte>(num_[j]) << MSR_BITS_PER_BYTE) >> div.rem) + d;
                num_[j] = static_cast<byte>(d >> MSR_BITS_PER_BYTE);
                d <<= MSR_BITS_PER_BYTE;
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

    large_int &large_int::operator&=(const large_int &another) noexcept {
        auto size = std::min(num_.size(), another.num_.size());
        if (num_.size() > size) {
            num_.resize(size);
        }
        for (size_t i = 0; i < size; ++i) {
            num_[i] &= another.num_[i];
        }
        decltype(num_)::reverse_iterator p;
        for (p = num_.rbegin(); p != num_.rend() && *p == 0; ++p);
        if (p == num_.rend()) {
            signed_ = false;
            num_ = {0};
        } else {
            num_.resize(num_.rend() - p);
        }
        return *this;
    }

    large_int &large_int::operator|=(const large_int &another) noexcept {
        auto size = std::min(num_.size(), another.num_.size());
        for (size_t i = 0; i < size; ++i) {
            num_[i] |= another.num_[i];
        }
        decltype(num_)::reverse_iterator p;
        for (p = num_.rbegin(); p != num_.rend() && *p == 0; ++p);
        if (p == num_.rend()) {
            signed_ = false;
            num_ = {0};
        } else {
            num_.resize(num_.rend() - p);
        }
        return *this;
    }

    large_int &large_int::operator^=(const large_int &another) noexcept {
        auto size = std::min(num_.size(), another.num_.size());
        auto another_size = another.num_.size();
        if (num_.size() < another_size) {
            num_.resize(another_size);
        }
        for (size_t i = 0; i < size; ++i) {
            num_[i] ^= i < another_size ? another.num_[i] : 0;
        }
        decltype(num_)::reverse_iterator p;
        for (p = num_.rbegin(); p != num_.rend() && *p == 0; ++p);
        if (p == num_.rend()) {
            signed_ = false;
            num_ = {0};
        } else {
            num_.resize(num_.rend() - p);
        }
        return *this;
    }

    large_int &large_int::operator+=(const self_type &another) noexcept {
        auto &a = num_;
        auto &b = another.num_;
        if (signed_ ^ another.signed_ && another) {
            return *this -= -another;
        }
        size_t max_size = std::max(a.size(), b.size());
        if (a.size() < max_size) {
            a.resize(max_size);
        }
        dual_byte c = 0;
        for (size_t i = 0; i < max_size; ++i) {
            dual_byte ub = 0;
            if (i < b.size()) {
                ub = b[i];
            } else if (!c) {
                break;
            }
            c = static_cast<dual_byte>(a[i]) + ub + c;
            a[i] = static_cast<byte>(c);
            c >>= MSR_BITS_PER_BYTE;
        }
        if (c) {
            a.push_back(static_cast<byte>(c));
        }
        return *this;
    }

    large_int &large_int::operator-=(const self_type &another) noexcept {
        auto &a = num_;
        auto &b = another.num_;
        if (signed_ ^ another.signed_ && another) {
            return *this += -another;
        }
        size_t max_size = std::max(a.size(), b.size());
        while (a.size() < max_size) {
            a.push_back(0);
        }
        dual_byte c = 0;
        for (size_t i = 0; i < max_size; ++i) {
            c ^= byte_all_one;
            c++;
            c &= byte_all_one;
            dual_byte ub = 0;
            if (i < b.size()) {
                ub = b[i];
            } else if (!c) {
                break;
            }
            c = static_cast<dual_byte>(a[i]) - ub - c;
            a[i] = static_cast<byte>(c);
            c >>= MSR_BITS_PER_BYTE;
        }
        if (c) {
            for (auto &i : a) {
                i ^= byte_all_one;
            }
            auto signed__ = signed_;
            signed_ = false;
            operator++();
            signed_ = !signed__;
        }
        decltype(num_)::reverse_iterator p;
        for (p = num_.rbegin(); p != num_.rend() && *p == 0; ++p);
        if (p == num_.rend()) {
            signed_ = false;
            num_ = {0};
        } else {
            num_.resize(num_.rend() - p);
        }
        return *this;
    }

    large_int &large_int::operator*=(const self_type &another) noexcept {
        auto a = num_;
        auto &b = another.num_;
        signed_ = signed_ ^ another.signed_;
        num_ = decltype(num_)(a.size() + b.size());
        decltype(num_) part(a.size() + b.size());
        for (size_t i = 0; i < b.size(); ++i) {
            memset(&part[0], 0, part.size() * sizeof(byte));
            dual_byte c = 0;
            for (size_t j = 0; j < a.size(); ++j) {
                c = static_cast<dual_byte>(a[j]) * static_cast<dual_byte>(b[i]) + c;
                part[i + j] = static_cast<byte>(c);
                c >>= MSR_BITS_PER_BYTE;
            }
            if (c) {
                part[i + a.size()] = static_cast<byte>(c);
            }
            c = 0;
            for (size_t j = 0; j < num_.size(); ++j) {
                c = static_cast<dual_byte>(num_[j]) + part[j] + c;
                num_[j] = static_cast<byte>(c);
                c >>= MSR_BITS_PER_BYTE;
            }
        }
        decltype(num_)::reverse_iterator p;
        for (p = num_.rbegin(); p != num_.rend() && *p == 0; ++p);
        if (p == num_.rend()) {
            signed_ = false;
            num_ = {0};
        } else {
            num_.resize(num_.rend() - p);
        }
        return *this;
    }

    large_int &large_int::operator/=(const self_type &another) throw(divide_by_zero) {
        if (!another) {
            throw divide_by_zero(MSR_FUNCTION_SIGNATURE, "");
        }
        signed_ = signed_ ^ another.signed_;
        self_type a = abs(*this), b = abs(another);
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

    large_int &large_int::operator%=(const self_type &another) throw(divide_by_zero) {
        auto d = div(*this, another);
        *this = d.rem;
        return *this;
    }

    bool operator==(const large_int &a, const large_int &b) noexcept {
        return !(a != b);
    }

    bool operator!=(const large_int &a, const large_int &b) noexcept {
        if ((a.signed_ ^ b.signed_) || a.num_.size() != b.num_.size()) {
            return true;
        }
        for (size_t i = 0; i < a.num_.size(); ++i) {
            if (a.num_[i] != b.num_[i]) {
                return true;
            }
        }
        return false;
    }

    bool operator<(const large_int &a, const large_int &b) noexcept {
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

    bool operator>(const large_int &a, const large_int &b) noexcept {
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

    bool operator<=(const large_int &a, const large_int &b) noexcept {
        return !(a > b);
    }

    bool operator>=(const large_int &a, const large_int &b) noexcept {
        return !(a < b);
    }

    large_int operator<<(const large_int &a, const size_t &b) noexcept {
        auto c = a;
        return std::move(c <<= b);
    }

    large_int operator>>(const large_int &a, const size_t &b) noexcept {
        auto c = a;
        return std::move(c >>= b);
    }

    large_int operator&(const large_int &a, const large_int &b) noexcept {
        auto c = a;
        return std::move(c &= b);
    }

    large_int operator|(const large_int &a, const large_int &b) noexcept {
        auto c = a;
        return std::move(c |= b);
    }

    large_int operator^(const large_int &a, const large_int &b) noexcept {
        auto c = a;
        return std::move(c ^= b);
    }

    large_int operator+(const large_int &a, const large_int &b) noexcept {
        large_int c = a;
        return std::move(c += b);
    }

    large_int operator-(const large_int &a, const large_int &b) noexcept {
        large_int c = a;
        return std::move(c -= b);
    }

    large_int operator*(const large_int &a, const large_int &b) noexcept {
        auto c = a;
        return std::move(c *= b);
    }

    large_int operator/(const large_int &a, const large_int &b) throw(divide_by_zero) {
        auto c = a;
        return std::move(c /= b);
    }

    large_int operator%(const large_int &a, const large_int &b) throw(divide_by_zero) {
        auto c = a;
        return std::move(c %= b);
    }

    large_int::operator bool() const noexcept {
        return num_.size() != 1 || *num_.rbegin() != 0;
    }

    template <unsigned int Ary, class Char, class C = typename std::enable_if<(Ary > 1)>::type>
    std::basic_ostream<Char> &output(std::basic_ostream<Char> &os, const large_int &n) noexcept {
        using namespace detail;
        if (n.signed_) {
            os << '-';
        }
        const size_t length = MSR_BITS_PER_BYTE * n.num_.size() / (bit_size<Ary>::value - 1) + 2;
        std::vector<digit<Ary>> sum(length, 0u);
        std::vector<digit<Ary>> pow(length, 0u);
        pow[0]++;
        size_t pow_size = 1;
        auto bit_of = [&](const size_t &i) {
            auto div = std::lldiv(i, MSR_BITS_PER_BYTE);
            auto quot = static_cast<size_t>(div.quot);
            auto rem = static_cast<byte>(div.rem);
            return n.num_[quot] & (1 << rem);
        };
        for (size_t i = 0; i < n.num_.size() * MSR_BITS_PER_BYTE; ++i) {
            if (bit_of(i)) {
                sum[0] += pow[0];
                auto carry = sum[0].carry();
                auto last_carry = carry;
                for (size_t j = 1; j < pow_size; ++j) {
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
            for (size_t j = 1; j < pow_size; ++j) {
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
        ++i;
        std::basic_stringstream<Char> ss;
        ss.flags(os.flags());
        for (; i > 0; i--) {
            auto j = i - 1;
            ss << sum[j];
        }
        os << ss.str();
        return os;
    }

    template <class Char>
    std::basic_ostream<Char> &operator<<(std::basic_ostream<Char> &os, const large_int &n) noexcept {
        const auto &flags = os.flags();
        if (flags & std::ios::oct) {
            return output<8>(os, n);
        } else if (flags & std::ios::dec) {
            return output<10>(os, n);
        } else if (flags & std::ios::hex) {
            return output<16>(os, n);
        }
        return os;
    }

}

#endif
