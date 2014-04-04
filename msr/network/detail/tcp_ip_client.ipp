#ifndef MSR_NETWORK_TCP_IP_CLIENT_IPP_INCLUDED
#define MSR_NETWORK_TCP_IP_CLIENT_IPP_INCLUDED

#include <boost/lexical_cast.hpp>

namespace msr {
    namespace network {
        client<protocol::ip::tcp>::client() {
            service_.stop();
        }
        void client<protocol::ip::tcp>::connect(std::string host,
                                                std::uint16_t port) {
            using namespace std;
            using namespace boost::asio;
            using ip::tcp;
            auto port_string = boost::lexical_cast<std::string>(port);
            auto c = make_shared<connection>(service_);
            tcp::resolver resolver(service_);
            async_connect(c->socket_, resolver.resolve({host, port_string}),
                          [c, this](error e, tcp::resolver::iterator i) {
                broadcast(&observer::client_did_connect, c, e);
            });
        }
        void client<protocol::ip::tcp>::send(connection_ptr c, data d) {
            using namespace std;
            using namespace boost::asio;
            if (!c) {
                return;
            }
            auto data_ = make_shared<data>(d);
            async_write(c->socket_, data_->const_buffer(),
                        [c, this, data_](error e, size_t size) {
                if (e) {
                    connections_.remove(c);
                }
                broadcast(&observer::client_did_send, c, e, *data_);
            });
        }
        void client<protocol::ip::tcp>::receive(connection_ptr c,
                                                std::size_t size) {
            using namespace std;
            using namespace boost::asio;
            if (!c) {
                return;
            }
            auto data_ = make_shared<data>(size);
            async_read(c->socket_, data_->mutable_buffer(),
                       [c, this, data_](error e, size_t size) {
                if (e) {
                    connections_.remove(c);
                }
                broadcast(&observer::client_did_receive, c, e, *data_);
            });
        }
        void client<protocol::ip::tcp>::cancel(connection_ptr c) {
            if (!c) {
                return;
            }
            error e;
            c->socket_.cancel(e);
            if (e) {
                connections_.remove(c);
            }
            service_.post([this, c, e]() {
                broadcast(&observer::client_did_cancel, c, e);
            });
        }
        void client<protocol::ip::tcp>::disconnect(connection_ptr c) {
            if (!c) {
                return;
            }
            connections_.remove(c);
            error e;
            protocol::ip::tcp::endpoint p;
            try {
                p = c->remote_endpoint();
            } catch (...) {}
            c->socket_.close(e);
            if (e) {
                connections_.remove(c);
            }
            service_.post([this, e, p]() {
                broadcast(&observer::client_did_disconnect, p, e);
            });
        }
        void client<protocol::ip::tcp>::run() {
            using namespace boost::asio;
            if (!is_running()) {
                service_.reset();
                for (std::size_t i = 0; i < pool_.size(); i++) {
                    pool_.post([this]() {
                        io_service::work work(service_);
                        service_.run();
                    });
                }
            }
            service_.post([this]() {
                broadcast(&observer::client_did_run);
            });
        }
        void client<protocol::ip::tcp>::shutdown() {
            using namespace boost::asio;
            using ip::tcp;
            for (auto &c : connections_) {
                c->socket_.shutdown(tcp::socket::shutdown_both);
            }
            connections_.clear();
            service_.stop();
            pool_.post([this]() {
                broadcast(&observer::client_did_shutdown);
            });
        }
        bool client<protocol::ip::tcp>::is_running() const {
            return !service_.stopped();
        }
        bool client<protocol::ip::tcp>::is_shutdown() const {
            return service_.stopped();
        }
        client<protocol::ip::tcp>::~client() {
            using namespace boost::asio;
            using ip::tcp;
            for (auto &c : connections_) {
                c->socket_.shutdown(tcp::socket::shutdown_both);
            }
            connections_.clear();
            service_.stop();
        }
    }
}

#endif

