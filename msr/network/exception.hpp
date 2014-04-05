#ifndef MSR_NETWORK_EXCEPTION_HPP_INCLUDED
#define MSR_NETWORK_EXCEPTION_HPP_INCLUDED

#include <msr/exception.hpp>

namespace msr {
    namespace network {
        class exception: public msr::exception {
        public:
            using self = exception;
            using base = msr::exception;
            using id_t = std::uint16_t;
        public:
            exception(const id_t &base_id, const id_t &id, const char *prefix, const char *what = "(null)"):
                base(0x10010000, static_cast<id_t>(base_id + id), (std::string("network::") + prefix).c_str(), what) {}
        };
    }
}

#endif

