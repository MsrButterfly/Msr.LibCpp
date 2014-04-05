#ifndef MSR_NETWORK_TCP_IP_SERVER_HPP_INCLUDED
#define MSR_NETWORK_TCP_IP_SERVER_HPP_INCLUDED

#include <list>
#include <memory>
#include <boost/asio.hpp>
#include <msr/network/data.hpp>
#include <msr/network/protocol.hpp>
#include <msr/network/server.hpp>
#include <msr/network/tcp_ip_connection.hpp>
#include <msr/thread.hpp>

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
            using connection_ptr = std::shared_ptr<connection>;
        public:
            server(const protocol::ip::tcp::endpoint &endpoint);
        public:
            void accept();
            void send(connection_ptr c, data d);
            void receive(connection_ptr c, std::size_t size);
            void disconnect(connection_ptr c);
            void cancel(connection_ptr c);
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
            std::list<connection_ptr> connections_;
        };
    }
}

#include <msr/network/tcp_ip_server_observer.hpp>

#endif
