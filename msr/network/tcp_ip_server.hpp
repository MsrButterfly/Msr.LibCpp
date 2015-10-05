#ifndef MSR_NETWORK_TCP_IP_SERVER_HPP_INCLUDED
#define MSR_NETWORK_TCP_IP_SERVER_HPP_INCLUDED

#include <memory>
#include <set>
#include <boost/asio.hpp>
#include <boost/utility.hpp>
#include <msr/network/data.hpp>
#include <msr/network/protocol.hpp>
#include <msr/network/tcp_ip_connection.hpp>
#include <msr/thread.hpp>
#include <msr/utility.hpp>

namespace msr {
    namespace network {
        class tcp_ip_server_observer;
        class tcp_ip_server: boost::noncopyable, public observable {
            MSR_CLASS_TYPE_DEFINATIONS(tcp_ip_server);
            MSR_OBSERVER_TYPE(tcp_ip_server_observer);
        public:
            using connection = tcp_ip_connection;
            using endpoint = protocol::ip::tcp::endpoint;
        private:
            using mutex = boost::shared_mutex;
            using readwrite_lock = boost::unique_lock<mutex>;
            using readonly_lock = boost::shared_lock<mutex>;
        public:
            tcp_ip_server(const endpoint &p);
        public:
            void accept();
            void send(connection::shared_ptr c, data d);
            void receive(connection::shared_ptr c, size_t size);
            void disconnect(connection::shared_ptr c);
#if BOOST_ASIO_ENABLE_CANCELIO
            void cancel(connection::shared_ptr c);
#endif
            void run();
            void shutdown();
            bool is_running() const;
            bool is_shutdown() const;
        public:
            ~tcp_ip_server();
        private:
            thread_pool pool_;
            boost::asio::io_service service_;
            protocol::ip::tcp::acceptor acceptor_;
            endpoint endpoint_;
            std::set<connection::shared_ptr> connections_;
        };
    }
}

#include <msr/network/tcp_ip_server_observer.hpp>

#endif
