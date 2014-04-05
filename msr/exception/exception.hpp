#ifndef MSR_EXCEPTION_EXCEPTION_HPP_INCLUDED
#define MSR_EXCEPTION_EXCEPTION_HPP_INCLUDED

#include <exception>
#include <string>
#include <msr/compatibility/exception.hpp>

namespace msr {
    class exception {
    public:
        using self = exception;
        using id_t = std::uint32_t;
    public:
        exception(const id_t &base_id, const id_t &id, const char *prefix, const char *what = "(null)"):
            id_(base_id + id), what_(what), prefix_(std::string("msr::") + prefix) {}
        exception(const self &another):
            id_(another.id_), prefix_(another.prefix_), what_(another.what_) {}
        exception(self &&another):
            id_(std::move(another.id_)), prefix_(std::move(another.prefix_)), what_(std::move(another.what_)) {}
    public:
        self &operator=(const self &another) {
            id_ = another.id_;
            prefix_ = another.prefix_;
            what_ = another.what_;
            return *this;
        }
        self &operator=(self &&another) {
            id_ = std::move(another.id_);
            prefix_ = std::move(another.prefix_);
            what_ = std::move(another.prefix_);
            return *this;
        }
        const bool operator==(const self &another) {
            return id_ == another.id_;
        }
        const id_t &id() const MSR_NOEXCEPT {
            return id_;
        }
        const char *const prefix() const MSR_NOEXCEPT {
            return prefix_.c_str();
        }
        const char *const what() const MSR_NOEXCEPT {
            return what_.c_str();
        }
    private:
        id_t id_;
        std::string prefix_;
        std::string what_;
    };
}

#endif

