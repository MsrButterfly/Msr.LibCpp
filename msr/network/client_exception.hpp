#ifndef MSR_NETWORK_CLIENT_EXCEPTION_HPP_INCLUDED
#define MSR_NETWORK_CLIENT_EXCEPTION_HPP_INCLUDED

#include <msr/network/exception.hpp>

namespace msr {
    namespace network {
        class client_exception: public exception {
        public:
            using self = client_exception;
            using base = exception;
            using id_t = uint8_t;
        public:
            client_exception(const id_t &id, const char *what = "(null)"):
                base(0x0100, id, "client_exception", what) {}
        };
    }
}

#endif
