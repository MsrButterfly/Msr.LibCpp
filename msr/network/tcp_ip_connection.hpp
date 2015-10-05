#ifndef MSR_NETWORK_TCP_IP_CONNECTION_HPP_INCLUDED
#define MSR_NETWORK_TCP_IP_CONNECTION_HPP_INCLUDED

#include <list>
#include <memory>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <msr/network/protocol.hpp>
#include <msr/utility.hpp>

namespace msr {
    namespace network {
        class tcp_ip_connection: boost::noncopyable {
            MSR_CLASS_TYPE_DEFINATIONS(tcp_ip_connection);
        public:
            friend class tcp_ip_server;
            friend class tcp_ip_client;
        private:
            using mutex = boost::shared_mutex;
            using readonly_lock = boost::shared_lock<mutex>;
            using readwrite_lock = boost::unique_lock<mutex>;
        public:
            tcp_ip_connection(boost::asio::io_service &io_service):
                socket_(io_service) {}
        public:
            protocol::ip::tcp::endpoint remote_endpoint() const {
                return socket_.remote_endpoint();
            }
        private:
            protocol::ip::tcp::socket socket_;
            mutex data_mutex_;
        };
    }
}

#endif
