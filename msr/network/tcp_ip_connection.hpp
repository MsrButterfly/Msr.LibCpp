#ifndef MSR_NETWORK_TCP_IP_CONNECTION_HPP_INCLUDED
#define MSR_NETWORK_TCP_IP_CONNECTION_HPP_INCLUDED

#include <list>
#include <memory>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <msr/network/server.hpp>
#include <msr/network/protocol.hpp>
#include <msr/network/connection.hpp>

namespace msr {
    namespace network {
        template <>
        class connection<protocol::ip::tcp>: public detail::connection_base {
            friend server<protocol::ip::tcp>;
        private:
            using mutex = boost::shared_mutex;
            using readonly_lock = boost::shared_lock<mutex>;
            using readwrite_lock = boost::unique_lock<mutex>;
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
            mutex data_mutex_;
        };
    }
}

#endif
