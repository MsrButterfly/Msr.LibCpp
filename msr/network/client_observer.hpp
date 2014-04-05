#ifndef MSR_NETWORK_CLIENT_OBSERVER_HPP_INCLUDED
#define MSR_NETWORK_CLIENT_OBSERVER_HPP_INCLUDED

#include <msr/network/client.hpp>
#include <msr/network/observer.hpp>

namespace msr {
    namespace network {
        template <class Protocol>
        class client;
        template <class Protocol>
        class observer<client<Protocol>>: public detail::observer_base {
        public:
            using self = observer;
            using base = observer_base;
            using server = network::client<Protocol>;
        };
    }
}

#endif
