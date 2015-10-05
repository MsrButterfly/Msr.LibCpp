#ifndef MSR_NETWORK_TCP_IP_SERVER_OBSERVER_HPP_INCLUDED
#define MSR_NETWORK_TCP_IP_SERVER_OBSERVER_HPP_INCLUDED

#include <memory>
#include <boost/asio.hpp>
#include <boost/utility.hpp>
#include <msr/network/data.hpp>
#include <msr/network/tcp_ip_server.hpp>
#include <msr/utility.hpp>

namespace msr {
    namespace network {
        class tcp_ip_server_observer: public observer {
            MSR_CLASS_TYPE_DEFINATIONS(tcp_ip_server_observer);
        public:
            using server = tcp_ip_server;
            using connection = tcp_ip_connection;
            using endpoint = protocol::ip::tcp::endpoint;
        public:
            virtual void server_did_accept(server::weak_ptr s, connection::shared_ptr c, error e) = 0;
            virtual void server_did_send(server::weak_ptr s, connection::shared_ptr c, error e, data d) = 0;
            virtual void server_did_receive(server::weak_ptr s, connection::shared_ptr c, error e, data d) = 0;
            virtual void server_did_disconnect(server::weak_ptr s, endpoint p, error e) = 0;
            virtual void server_did_cancel(server::weak_ptr s, connection::shared_ptr c, error e) = 0;
            virtual void server_did_run(server::weak_ptr s, error e) = 0;
            virtual void server_did_shutdown(server::weak_ptr s, error e) = 0;
        public:
            ~tcp_ip_server_observer() {}
        };
    }
}

#include <msr/network/detail/tcp_ip_server.ipp>

#endif
