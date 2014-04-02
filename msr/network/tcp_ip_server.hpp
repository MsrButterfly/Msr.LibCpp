#ifndef MSR_NETWORK_TCP_IP_SERVER_HPP_INCLUDED
#define MSR_NETWORK_TCP_IP_SERVER_HPP_INCLUDED

#include <list>
#include <memory>
#include <boost/asio.hpp>
#include <msr/network/protocol.hpp>
#include <msr/network/server.hpp>
#include <msr/network/tcp_ip_connection.hpp>
#include <msr/thread.hpp>
#include <msr/network/data.hpp>

namespace msr {
    namespace network {
        template <>
        class server<protocol::ip::tcp>: public detail::server_base {
        public:
            using self = server;
            using base = server_base;
            using connection = network::connection<protocol::ip::tcp>;
            using observer = network::observer<self>;
            using mutex = boost::shared_mutex;
            using readwrite_lock = boost::unique_lock<mutex>;
            using readonly_lock = boost::shared_lock<mutex>;
        public:
            server(const protocol::ip::tcp::endpoint &_endpoint):
            endpoint_(_endpoint), acceptor_(service_, _endpoint, true) {
                service_.stop();
            }
        public:
            void accept();
            void send(std::shared_ptr<connection> c, const data &d);
            void receive(std::shared_ptr<connection> c, const std::size_t &size);
            void close(std::shared_ptr<connection> c);
            void cancel(std::shared_ptr<connection> c);
            void run();
            void shutdown();
            bool is_running() const;
            bool is_shutdown() const;
        public:
            ~server();
        private:
            thread_pool pool_;
            boost::asio::io_service service_;
            protocol::ip::tcp::acceptor acceptor_;
            protocol::ip::tcp::endpoint endpoint_;
            std::list<std::shared_ptr<connection>> connections_;
        };
    }
}

#endif
