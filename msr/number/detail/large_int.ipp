#ifndef MSR_NUMBER_LARGE_INT_IPP_INCLUDED
#define MSR_NUMBER_LARGE_INT_IPP_INCLUDED

namespace msr {
    large_int::large_int()
    : signed_(false), num_{0} {}
    large_int::large_int(const self &another)
    : signed_(another.signed_), num_(another.num_) {}
    large_int::large_int(self &&another)
    : signed_(another.signed_), num_(std::move(another.num_)) {
        another.signed_ = false;
        another.num_ = {0};
    }
    template <class T, class C>
    large_int::large_int(const T &num)
    : signed_(std::is_signed<T>::value && num < 0) {
        largest_t n;
        n = signed_ ? -num : num;
        while (n > 0) {
            num_.push_back(static_cast<unit_t>(n));
            if (sizeof(n) <= sizeof(unit_t)) {
                break;
            } else {
                n >>= std::numeric_limits<unit_t>::digits;
            }
        }
    }
    large_int &large_int::operator=(const self &another) {
        signed_ = another.signed_;
        num_ = another.num_;
        return *this;
    }
    large_int &large_int::operator=(self &&another) {
        signed_ = another.signed_;
        num_ = another.num_;
        another.signed_ = false;
        another.num_ = {0};
        return *this;
    }
    bool operator==(const large_int &a, const large_int &b) {
        if (a.signed_ != b.signed_ || a.num_.size() != b.num_.size()) {
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
        if (a.signed_ != b.signed_ || a.num_.size() != b.num_.size()) {
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
        if (a.signed_ != b.signed_) {
            return a.signed_;
        }
        auto &signed_ = a.signed_;
        if (a.num_.size() != b.num_.size()) {
            return a.num_.size() < b.num_.size() != signed_;
        }
        for (auto i = a.num_.size(); i > 0; i--) {
            auto j = i - 1;
            if (a.num_[j] != b.num_[j]) {
                return (a.num_[j] < b.num_[j]) != signed_;
            }
        }
        return false;
    }
    bool operator>(const large_int &a, const large_int &b) {
        if (a.signed_ != b.signed_) {
            return b.signed_;
        }
        auto &signed_ = a.signed_;
        if (a.num_.size() != b.num_.size()) {
            return a.num_.size() > b.num_.size() != signed_;
        }
        for (auto i = a.num_.size(); i > 0; i--) {
            auto j = i - 1;
            if (a.num_[j] != b.num_[j]) {
                return (a.num_[j] > b.num_[j]) != signed_;
            }
        }
        return false;
    }
    bool operator<=(const large_int &a, const large_int &b) {
        if (a.signed_ != b.signed_) {
            return a.signed_;
        }
        auto &signed_ = a.signed_;
        if (a.num_.size() != b.num_.size()) {
            return a.num_.size() < b.num_.size() != signed_;
        }
        for (auto i = a.num_.size(); i > 0; i--) {
            auto j = i - 1;
            if (a.num_[j] != b.num_[j]) {
                return (a.num_[j] < b.num_[j]) != signed_;
            }
        }
        return true;
    }
    bool operator>=(const large_int &a, const large_int &b) {
        if (a.signed_ != b.signed_) {
            return a.signed_;
        }
        auto &signed_ = a.signed_;
        if (a.num_.size() != b.num_.size()) {
            return a.num_.size() < b.num_.size() != signed_;
        }
        for (auto i = a.num_.size(); i > 0; i--) {
            auto j = i - 1;
            if (a.num_[j] != b.num_[j]) {
                return (a.num_[j] < b.num_[j]) != signed_;
            }
        }
        return true;
    }
    large_int operator+(const large_int &a, const large_int &b) {
        large_int c = a;
        c += b;
        return c;
    }
    large_int &large_int::operator+=(const self &another) {
        auto &a = num_;
        auto &b = another.num_;
        if (signed_ != another.signed_) {
            return *this -= -another;
        }
        std::size_t max_size = std::max(a.size(), b.size());
        while (a.size() < max_size) {
            a.push_back(0);
        }
        dual_t c = 0;
        for (std::size_t i = 0; i < max_size; i++) {
            dual_t ub = i < b.size() ? b[i] : 0;
            c = static_cast<dual_t>(a[i]) + ub + c;
            a[i] = static_cast<unit_t>(c);
            c >>= std::numeric_limits<unit_t>::digits;
        }
        if (c) {
            a.push_back(static_cast<unit_t>(c));
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
    large_int &large_int::operator-=(const self &another) {
        auto &a = num_;
        auto &b = another.num_;
        if (signed_ != another.signed_) {
            return *this += -another;
        }
        std::size_t max_size = std::max(a.size(), b.size());
        while (a.size() < max_size) {
            a.push_back(0);
        }
        dual_t c = 0;
        for (std::size_t i = 0; i < max_size; i++) {
            c ^= std::numeric_limits<unit_t>::max();
            c++;
            c &= std::numeric_limits<unit_t>::max();
            dual_t ub = i < b.size() ? b[i] : 0;
            c = static_cast<dual_t>(a[i]) - ub - c;
            a[i] = static_cast<unit_t>(c);
            c >>= std::numeric_limits<unit_t>::digits;
        }
        if (c) {
            for (auto &i : a) {
                i ^= std::numeric_limits<unit_t>::max();
            }
            auto signed__ = signed_;
            signed_ = false;
            operator++();
            signed_ = !signed__;
        }
        while (a.size() > 1 && *a.rbegin() == 0) {
            a.pop_back();
        }
        if (a.size() == 1 && a[0] == 0) {
            signed_ = false;
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
    template <class Char>
    std::basic_ostream<Char> &operator<<(std::basic_ostream<Char> &os, const large_int &n) {
        auto m = n;
        if (m.signed_) {
            os << '-';
        }
        large_int::largest_t s = 0;
        for (int i = 0; i < n.num_.size(); i++) {
            s += std::pow(std::numeric_limits<large_int::unit_t>::max() + 1, i) * n.num_[i];
        }
        os << s;
        return os;
    }
}

#endif
