#ifndef MSR_NETWORK_DATA_EXCEPTION_HPP
#define MSR_NETWORK_DATA_EXCEPTION_HPP

#include <msr/network/exception.hpp>

namespace msr {
    namespace network {
        class data_exception: public exception {
        public:
            using self = data_exception;
            using base = exception;
            using id_t = uint8_t;
        public:
            data_exception(const id_t &id, const char *what = "(null)"):
                base(0x0200, id, "data_exception", what) {}
        };
    }
}

#endif

