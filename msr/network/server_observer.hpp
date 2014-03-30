#ifndef MSR_NETWORK_SERVER_OBSERVER_HPP_INCLUDED
#define MSR_NETWORK_SERVER_OBSERVER_HPP_INCLUDED

#include <memory>
#include <msr/network/connection.hpp>
#include <msr/network/protocol.hpp>
#include <msr/network/observer.hpp>

namespace msr {
    namespace network {
        template <class Protocol>
        class server;
        template <class Protocol>
        class observer<server<Protocol>>: public detail::observer_base {
        public:
            using self = observer;
            using base = observer_base;
            using server = network::server<Protocol>;
        };
    }
}

#endif
