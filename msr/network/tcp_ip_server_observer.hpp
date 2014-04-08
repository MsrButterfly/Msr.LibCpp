#ifndef MSR_NETWORK_TCP_IP_SERVER_OBSERVER_HPP_INCLUDED
#define MSR_NETWORK_TCP_IP_SERVER_OBSERVER_HPP_INCLUDED

#include <memory>
#include <boost/asio.hpp>
#include <msr/network/data.hpp>
#include <msr/network/server_observer.hpp>
#include <msr/network/tcp_ip_server.hpp>

namespace msr {
    namespace network {
        template <>
        class observer<server<protocol::ip::tcp>>: public detail::observer_base {
        public:
            using self = observer;
            using base = observer_base;
            using server = network::server<protocol::ip::tcp>;
            using connection = server::connection;
            using server_weak_ptr = std::weak_ptr<server>;
            using server_shared_ptr = std::shared_ptr<server>;
            using connection_ptr = server::connection_ptr;
            using error = network::error;
            using data = network::data;
            using endpoint = protocol::ip::tcp::endpoint;
        public:
            virtual void server_did_accept(server_weak_ptr s, connection_ptr c, error e) = 0;
            virtual void server_did_send(server_weak_ptr s, connection_ptr c, error e, data d) = 0;
            virtual void server_did_receive(server_weak_ptr s, connection_ptr c, error e, data d) = 0;
            virtual void server_did_disconnect(server_weak_ptr s, endpoint p, error e) = 0;
            virtual void server_did_cancel(server_weak_ptr s, connection_ptr c, error e) = 0;
            virtual void server_did_run(server_weak_ptr s, error e) = 0;
            virtual void server_did_shutdown(server_weak_ptr s, error e) = 0;
        protected:
            server_shared_ptr lock(server_weak_ptr s) {
                try {
                    return s.lock();
                } catch (...) {
                    return nullptr;
                }
            }
        public:
            ~observer() {}
        };
    }
}

#include <msr/network/detail/tcp_ip_server.ipp>

#endif
