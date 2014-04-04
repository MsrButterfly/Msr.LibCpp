#ifndef MSR_NETWORK_CLIENT_HPP_INCLUDED
#define MSR_NETWORK_CLIENT_HPP_INCLUDED

#include <memory>
#include <msr/network/protocol.hpp>
#include <msr/network/detail/client_base.hpp>

namespace msr {
    namespace network {
        template <class T>
        struct is_client: public boost::is_base_of<detail::client_base, T> {};
        template <class Protocol>
        class client: public detail::client_base {
        public:
            using self = client;
            using base = client_base;
        };
    }
}

#endif
