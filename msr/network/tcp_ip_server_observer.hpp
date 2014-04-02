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
        public:
            virtual void did_accept(std::weak_ptr<server> s, std::shared_ptr<connection> c, error e) = 0;
            virtual void did_send(std::weak_ptr<server> s, std::shared_ptr<connection> c, error e, data d) = 0;
            virtual void did_receive(std::weak_ptr<server> s, std::shared_ptr<connection> c, error e, data d) = 0;
            virtual void did_run(std::weak_ptr<server> s, error e) = 0;
            virtual void did_shutdown(std::weak_ptr<server> s, error e) = 0;
        public:
            ~observer() {}
        };
    }
}

#include <msr/network/tcp_ip_server.ipp>

#endif
