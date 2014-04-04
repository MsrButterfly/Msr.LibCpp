#ifndef MSR_NETWORK_CLIENT_EXCEPTION_HPP_INCLUDED
#define MSR_NETWORK_CLIENT_EXCEPTION_HPP_INCLUDED

#include <exception>
#include <string>
#include <msr/compatibility/exception.hpp>

namespace msr {
    namespace network {
        class client_exception: public std::exception {
        public:
            using self = client_exception;
            using base = exception;
        public:
            client_exception():
                _what("client_exception: (null description)") {}
            client_exception(const std::string &what):
                _what("client_exception: " + what) {};
            client_exception(const self &e): _what(e.what()) {};
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
