#ifndef MSR_NETWORK_TCP_IP_SERVER_HPP_INCLUDED
#define MSR_NETWORK_TCP_IP_SERVER_HPP_INCLUDED

#include <list>
#include <memory>
#include <boost/asio.hpp>
#include <msr/network/protocol.hpp>
#include <msr/network/server.hpp>
#include <msr/network/connection.hpp>
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
        public:
            server(const protocol::ip::tcp::endpoint &_endpoint):
                acceptor_(service_, _endpoint, true) {
                for (unsigned int i = 0; i < pool_.size(); i++) {
                    pool_.post([this]() {
                        boost::asio::io_service::work work(service_);
                        service_.run();
                    });
                }
            }
        public:
            void accept();
            template <class ConstBufferSequence>
            void send(std::shared_ptr<connection> c, ConstBufferSequence &buffer);
            template <class MutableBufferSequence>
            void receive(std::shared_ptr<connection> c, MutableBufferSequence &buffer);
        public:
            ~server() {
                service_.stop();
            }
        private:
            thread_pool pool_;
            boost::asio::io_service service_;
            protocol::ip::tcp::acceptor acceptor_;
            std::list<std::shared_ptr<connection>> connections_;
        };
    }
}

#endif
