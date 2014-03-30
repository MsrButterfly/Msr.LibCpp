#ifndef MSR_NETWORK_SERVER_EXCEPTION_HPP_INCLUDED
#define MSR_NETWORK_SERVER_EXCEPTION_HPP_INCLUDED

#include <exception>
#include <string>
#include <msr/compatibility/exception.hpp>

namespace msr {
    namespace network {
        class server_exception: public std::exception {
        public:
            using self = server_exception;
            using base = exception;
        public:
            server_exception():
                _what("server_exception: (null description)") {}
            server_exception(const std::string &what):
                _what("server_exception: " + what) {};
            server_exception(const self &e): _what(e.what()) {};
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
