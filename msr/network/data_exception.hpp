#ifndef MSR_NETWORK_DATA_EXCEPTION_HPP
#define MSR_NETWORK_DATA_EXCEPTION_HPP

#include <exception>
#include <string>
#include <msr/compatibility/exception.hpp>

namespace msr {
    namespace network {
        class data_exception: public std::exception {
        public:
            using self = data_exception;
            using base = exception;
        public:
            data_exception():
                _what("data_exception: (null description)") {}
            data_exception(const std::string &what):
                _what("data_exception: " + what) {};
            data_exception(const self &e): _what(e.what()) {};
        public:
            self &operator=(const self &e) {
                _what = e.what();
                return *this;
            }
            virtual const char *what() const MSR_NOEXCEPT override {
                return _what.c_str();
            }
        private:
            std::string _what;
        };
    }
}

#endif

