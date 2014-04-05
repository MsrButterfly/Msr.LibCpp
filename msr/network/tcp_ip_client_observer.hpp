#ifndef MSR_NETWORK_TCP_IP_CLIENT_OBSERVER_HPP_INCLUDED
#define MSR_NETWORK_TCP_IP_CLIENT_OBSERVER_HPP_INCLUDED

#include <memory>
#include <boost/asio.hpp>
#include <msr/network/client_observer.hpp>
#include <msr/network/data.hpp>
#include <msr/network/tcp_ip_client.hpp>

namespace msr {
    namespace network {
        template <>
        class observer<client<protocol::ip::tcp>>: public detail::observer_base {
        public:
            using self = observer;
            using base = observer_base;
            using client = network::client<protocol::ip::tcp>;
            using connection = client::connection;
            using client_weak_ptr = std::weak_ptr<client>;
            using client_shared_ptr = std::shared_ptr<client>;
            using connection_ptr = client::connection_ptr;
            using error = network::error;
            using data = network::data;
            using endpoint = protocol::ip::tcp::endpoint;
        public:
            virtual void client_did_connect(client_weak_ptr c, connection_ptr n, error e) = 0;
            virtual void client_did_send(client_weak_ptr c, connection_ptr n, error e, data d) = 0;
            virtual void client_did_receive(client_weak_ptr c, connection_ptr n, error e, data d) = 0;
            virtual void client_did_disconnect(client_weak_ptr c, endpoint p, error e) = 0;
            virtual void client_did_cancel(client_weak_ptr c, connection_ptr n, error e) = 0;
            virtual void client_did_run(client_weak_ptr c) = 0;
            virtual void client_did_shutdown(client_weak_ptr c) = 0;
        public:
            ~observer() {}
        };
    }
}

#include <msr/network/detail/tcp_ip_client.ipp>

#endif
