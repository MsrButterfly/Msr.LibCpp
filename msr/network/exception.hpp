#ifndef MSR_NETWORK_EXCEPTION_HPP_INCLUDED
#define MSR_NETWORK_EXCEPTION_HPP_INCLUDED

#include <msr/exception.hpp>
#include <msr/utility.hpp>

namespace msr {
    namespace network {
        class exception: public msr::exception {
            MSR_CLASS_TYPE_DEFINATIONS(exception);
        public:
            using base_type = msr::exception;
            using id_t = std::uint16_t;
        public:
            exception(const id_t &id, const char *module, const char *description = "(undefined)")
            : base_type(0x10010000 + id, module, description) {}
        };
    }
}

#endif

