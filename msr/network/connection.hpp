#ifndef MSR_NETWORK_CONNECTION_HPP_INCLUDED
#define MSR_NETWORK_CONNECTION_HPP_INCLUDED

#include <msr/network/detail/connection_base.hpp>

namespace msr {
    namespace network {
        template <class Protocol>
        class connection: public detail::connection_base {
        public:
            using self = connection;
            using base = connection_base;
        public:
            virtual ~connection() {}
        };
    }
}

#endif
