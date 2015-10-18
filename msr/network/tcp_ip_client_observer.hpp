#ifndef MSR_NETWORK_TCP_IP_CLIENT_OBSERVER_HPP_INCLUDED
#define MSR_NETWORK_TCP_IP_CLIENT_OBSERVER_HPP_INCLUDED

#include <memory>
#include <boost/asio.hpp>
#include <msr/network/data.hpp>
#include <msr/network/tcp_ip_client.hpp>
#include <msr/utility.hpp>

namespace msr {
    namespace network {
        
        class tcp_ip_client_observer: public observer {
            MSR_CLASS_TYPE_DEFINATIONS(tcp_ip_client_observer);
        public:
            using client = tcp_ip_client;
            using connection = tcp_ip_connection;
            using endpoint = protocol::ip::tcp::endpoint;
        public:
            virtual void client_did_connect(client::weak_ptr c, connection::shared_ptr n, error e) = 0;
            virtual void client_did_send(client::weak_ptr c, connection::shared_ptr n, error e, data d) = 0;
            virtual void client_did_receive(client::weak_ptr c, connection::shared_ptr n, error e, data d) = 0;
            virtual void client_did_disconnect(client::weak_ptr c, endpoint p, error e) = 0;
            virtual void client_did_cancel(client::weak_ptr c, connection::shared_ptr n, error e) = 0;
            virtual void client_did_run(client::weak_ptr c) = 0;
            virtual void client_did_shutdown(client::weak_ptr c) = 0;
        public:
            ~tcp_ip_client_observer() {}
        };
        
    }
}

#include <msr/network/detail/tcp_ip_client.ipp>

#endif
