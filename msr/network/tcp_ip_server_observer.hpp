#ifndef MSR_NETWORK_TCP_IP_SERVER_OBSERVER_HPP_INCLUDED
#define MSR_NETWORK_TCP_IP_SERVER_OBSERVER_HPP_INCLUDED

#include <memory>
#include <boost/asio.hpp>
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
        public:
            virtual void did_accept(std::weak_ptr<server> s, std::shared_ptr<connection> c, error e) = 0;
            virtual void did_send(std::weak_ptr<server> s, std::shared_ptr<connection> c, error e, std::size_t size) = 0;
            virtual void did_receive(std::weak_ptr<server> s, std::shared_ptr<connection> c, error e, std::size_t size) = 0;
        public:
            ~observer() {}
        };
        void server<protocol::ip::tcp>::accept() {
            auto next_connection = std::make_shared<connection>(service_);
            acceptor_.async_accept(next_connection->socket_, [next_connection, this](error e) mutable {
                if (!e) {
                    connections_.push_back(next_connection);
                } else {
                    next_connection = nullptr;
                }
                broadcast(&observer::did_accept, next_connection, e);
            });
        }
        template <class ConstBufferSequence>
        void server<protocol::ip::tcp>::send(std::shared_ptr<connection> c, ConstBufferSequence &buffer) {
            boost::asio::async_write(c->socket_, buffer, [c, this](error e, std::size_t size) {
                broadcast(&observer::did_send, c, e, size);
            });
        }
        template <class MutableBufferSequence>
        void server<protocol::ip::tcp>::receive(std::shared_ptr<connection> c, MutableBufferSequence &buffer) {
            boost::asio::async_read(c->socket_, buffer, [c, this](error e, std::size_t size) {
                broadcast(&observer::did_receive, c, e, size);
            });
        }
    }
}

#endif