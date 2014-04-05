#ifndef MSR_NETWORK_SERVER_EXCEPTION_HPP_INCLUDED
#define MSR_NETWORK_SERVER_EXCEPTION_HPP_INCLUDED

#include <msr/network/exception.hpp>

namespace msr {
    namespace network {
        class server_exception: public exception {
        public:
            using self = server_exception;
            using base = exception;
            using id_t = uint8_t;
        public:
            server_exception(const id_t &id, const char *what = "(null)"):
                base(0x0300, id, "server_exception", what) {}
        };
    }
}

#endif
