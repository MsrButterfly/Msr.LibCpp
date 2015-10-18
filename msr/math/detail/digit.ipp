#ifndef MSR_MATH_DETAIL_DIGIT_IPP_INCLUDED
#define MSR_MATH_DETAIL_DIGIT_IPP_INCLUDED

namespace msr {
    namespace detail {
    
        template <unsigned int Ary>
        digit<Ary>::digit()
        : digit(0u) {}
        
        template <unsigned int Ary>
        template <class T, class C>
        digit<Ary>::digit(const T &n)
        : carry_(0), borrow_(0), n_((n < Ary) ? static_cast<value_type>(n) : 0) {}
        
        template <unsigned int Ary>
        const typename digit<Ary>::value_type &digit<Ary>::get() const {
            return n_;
        }
        
        template <unsigned int Ary>
        const typename digit<Ary>::value_type &digit<Ary>::carry() const {
            return carry_;
        }
        
        template <unsigned int Ary>
        const typename digit<Ary>::value_type &digit<Ary>::borrow() const {
            return borrow_;
        }
        
        template <unsigned int Ary>
        digit<Ary> &digit<Ary>::operator=(const self_type &another) {
            carry_ = another.carry_;
            borrow_ = another.borrow_;
            n_ = another.n_;
            return *this;
        }
        
        template <unsigned int Ary>
        digit<Ary> &digit<Ary>::operator+=(const self_type &another) {
            carry_ = 0;
            borrow_ = 0;
            n_ += another.n_;
            if (n_ >= Ary) {
                n_ -= Ary;
                carry_ = 1;
            }
            return *this;
        }
        
        template <unsigned int Ary>
        digit<Ary> &digit<Ary>::operator++() {
            return operator+=(1u);
        }
        
        template <unsigned int Ary>
        digit<Ary> digit<Ary>::operator++(int) {
            auto a = *this;
            operator+=(1u);
            return a;
        }
        
        template <unsigned int Ary>
        digit<Ary> &digit<Ary>::operator-=(const self_type &another) {
            carry_ = 0;
            borrow_ = 0;
            if (n_ < another.n_) {
                n_ += Ary;
                borrow_ = 1;
            }
            n_ -= another.n_;
            return *this;
        }
        
        template <unsigned int Ary>
        digit<Ary> &digit<Ary>::operator--() {
            return operator-=(1u);
        }
        
        template <unsigned int Ary>
        digit<Ary> digit<Ary>::operator--(int) {
            auto a = *this;
            operator-=(1u);
            return a;
        }
        
        template <unsigned int Ary>
        digit<Ary> &digit<Ary>::operator*=(const self_type &another) {
            carry_ = 0;
            borrow_ = 0;
            n_ *= another.n_;
            carry_ = n_ / Ary;
            n_ -= carry_ * Ary;
            return *this;
        }
        
        template <unsigned int Ary>
        digit<Ary> &digit<Ary>::operator/=(const self_type &another) {
            carry_ = 0;
            borrow_ = 0;
            n_ /= another.n_;
            return *this;
        }
        
    }
}

#endif
