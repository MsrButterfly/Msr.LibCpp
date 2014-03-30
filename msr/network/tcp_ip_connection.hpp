#ifndef MSR_NETWORK_TCP_IP_CONNECTION_HPP_INCLUDED
#define MSR_NETWORK_TCP_IP_CONNECTION_HPP_INCLUDED

#include <boost/asio.hpp>
#include <msr/network/server.hpp>
#include <msr/network/protocol.hpp>
#include <msr/network/connection.hpp>

namespace msr {
    namespace network {
        template <>
        class connection<protocol::ip::tcp>: public detail::connection_base {
            friend server<protocol::ip::tcp>;
        public:
            using self = connection;
            using base = connection_base;
        public:
            connection(boost::asio::io_service &io_service):
                socket_(io_service) {}
            protocol::ip::tcp::socket::endpoint_type remote_endpoint() const {
                return socket_.remote_endpoint();
            }
        private:
            protocol::ip::tcp::socket socket_;
        };
    }
}

#endif
