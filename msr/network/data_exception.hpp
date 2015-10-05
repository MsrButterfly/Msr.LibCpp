#ifndef MSR_NETWORK_DATA_EXCEPTION_HPP
#define MSR_NETWORK_DATA_EXCEPTION_HPP

#include <msr/network/exception.hpp>
#include <msr/utility.hpp>

namespace msr {
    namespace network {
        class data_exception: public exception {
            MSR_CLASS_TYPE_DEFINATIONS(data_exception);
        public:
            using base_type = exception;
            using id_t = uint8_t;
        public:
            data_exception(const id_t &id, const char *module, const char *description = "(undefined)")
            : base_type(0x0200 + id, module, description) {}
        };
    }
}

#endif

