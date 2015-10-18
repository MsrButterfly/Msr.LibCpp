#ifndef MSR_EXCEPTION_EXCEPTION_HPP_INCLUDED
#define MSR_EXCEPTION_EXCEPTION_HPP_INCLUDED

#include <ostream>
#include <string>
#include <utility>
#include <msr/utility.hpp>

#if defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 180021114
#   pragma warning(disable: 4290)
#endif

namespace msr {
    
    class exception {
        MSR_CLASS_TYPE_DEFINATIONS(exception);
    public:
        exception(const char *func, const std::string &what)
        : what_(what), func_(func) {}
        exception(const self_type &another) {
            *this = another;
        }
        exception(self_type &&another) {
            *this = std::move(another);
        }
    public:
        self_type &operator=(const self_type &another) {
            func_ = another.func_;
            what_ = another.what_;
            return *this;
        }
        self_type &operator=(self_type &&another) {
            func_ = std::move(another.func_);
            what_ = std::move(another.what_);
            return *this;
        }
        virtual std::string function() const noexcept final {
            return func_;
        }
        virtual std::string what() const noexcept {
            return what_;
        }
    public:
        virtual ~exception() {}
    protected:
        std::string what_;
    private:
        std::string func_; // MSR_FUNCTION_SIGNATURE
    };
    
}

template <class Char>
std::basic_ostream<Char> &operator<<(std::basic_ostream<Char> &os, const msr::exception &e) {
    os << "[Exception]"
        << "\n    Type: " << typeid(e).name()
        << "\n    Function: " << e.function().c_str()
        << "\n    Description: " << e.what().c_str();
    return os;
}

#include <msr/exception/divide_by_zero.hpp>
#include <msr/exception/out_of_range.hpp>
#include <msr/exception/reverse_control.hpp>
#include <msr/exception/type_mismatch.hpp>

#endif
