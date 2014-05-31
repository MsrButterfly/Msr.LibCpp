#ifndef MSR_EXCEPTION_EXCEPTION_HPP_INCLUDED
#define MSR_EXCEPTION_EXCEPTION_HPP_INCLUDED

#include <exception>
#include <ios>
#include <string>
#include <msr/compatibility/exception.hpp>

namespace msr {
    class exception {
    public:
        using self_type = exception;
        using id_t = std::uint32_t;
    public:
        exception(const id_t &id, const char *module, const char *description = "(undefined)")
        : id_(id), description_(description), module_(module) {}
        exception(const self_type &another)
        : id_(another.id_), module_(another.module_), description_(another.description_) {}
        exception(self_type &&another)
        : id_(std::move(another.id_)), module_(std::move(another.module_)), description_(std::move(another.description_)) {}
    public:
        self_type &operator=(const self_type &another) {
            id_ = another.id_;
            module_ = another.module_;
            description_ = another.description_;
            return *this;
        }
        self_type &operator=(self_type &&another) {
            id_ = std::move(another.id_);
            module_ = std::move(another.module_);
            description_ = std::move(another.module_);
            return *this;
        }
        const bool operator==(const self_type &another) {
            return id_ == another.id_;
        }
        const id_t &id() const MSR_NOEXCEPT {
            return id_;
        }
        const char *const module() const MSR_NOEXCEPT {
            return module_.c_str();
        }
        const char *const description() const MSR_NOEXCEPT {
            return description_.c_str();
        }
        template <class Char>
        friend std::basic_ostream<Char> &operator<<(std::basic_ostream<Char> &os, const self_type &e) {
            os << '[' << e.id_ << "] " << e.module_ << ": " << e.description_;
            return os;
        }
    private:
        id_t id_;
        std::string module_;
        std::string description_;
    };
}

#endif

