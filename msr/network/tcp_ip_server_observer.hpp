#ifndef MSR_NETWORK_TCP_IP_SERVER_OBSERVER_HPP_INCLUDED
#define MSR_NETWORK_TCP_IP_SERVER_OBSERVER_HPP_INCLUDED

#include <memory>
#include <boost/asio.hpp>
#include <msr/network/server_observer.hpp>
#include <msr/network/tcp_ip_server.hpp>
#include <msr/network/data.hpp>

namespace msr {
    namespace network {
        template <>
        class observer<server<protocol::ip::tcp>>: public detail::observer_base {
        public:
            using self = observer;
            using base = observer_base;
            using server = network::server<protocol::ip::tcp>;
            using connection = server::connection;
            using server_ptr = std::weak_ptr<server>;
            using connection_ptr = std::shared_ptr<connection>;
            using error = network::error;
            using data = network::data;
            using endpoint = protocol::ip::tcp::endpoint;
        public:
            virtual void server_did_accept(server_ptr s, connection_ptr c, error e) = 0;
            virtual void server_did_send(server_ptr s, connection_ptr c, error e, data d) = 0;
            virtual void server_did_receive(server_ptr s, connection_ptr c, error e, data d) = 0;
            virtual void server_did_disconnect(server_ptr s, endpoint p, error e) = 0;
            virtual void server_did_cancel(server_ptr s, connection_ptr c, error e) = 0;
            virtual void server_did_run(server_ptr s, error e) = 0;
            virtual void server_did_shutdown(server_ptr s, error e) = 0;
        public:
            ~observer() {}
        };
    }
}

#include <msr/network/tcp_ip_server.ipp>

#endif
