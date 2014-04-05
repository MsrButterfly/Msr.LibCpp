#ifndef MSR_NETWORK_TCP_IP_CLIENT_HPP_INCLUDED
#define MSR_NETWORK_TCP_IP_CLIENT_HPP_INCLUDED

#include <list>
#include <memory>
#include <boost/asio.hpp>
#include <msr/network/client.hpp>
#include <msr/network/data.hpp>
#include <msr/network/protocol.hpp>
#include <msr/network/tcp_ip_connection.hpp>
#include <msr/thread.hpp>

namespace msr {
    namespace network {
        template <>
        class client<protocol::ip::tcp>: public detail::client_base {
        public:
            using self = client;
            using base = client_base;
            using connection = network::connection<protocol::ip::tcp>;
            using observer = network::observer<self>;
            using mutex = boost::shared_mutex;
            using readwrite_lock = boost::unique_lock<mutex>;
            using readonly_lock = boost::shared_lock<mutex>;
            using connection_ptr = std::shared_ptr<connection>;
        public:
            client();
        public:
            void connect(std::string host, std::uint16_t port);
            void send(connection_ptr c, data d);
            void receive(connection_ptr c, std::size_t size);
            void disconnect(connection_ptr c);
            void cancel(connection_ptr c);
            void run();
            void shutdown();
            bool is_running() const;
            bool is_shutdown() const;
        public:
            ~client();
        private:
            thread_pool pool_;
            boost::asio::io_service service_;
            std::list<connection_ptr> connections_;
        };
    }
}

#endif
