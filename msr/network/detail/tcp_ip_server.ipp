#ifndef MSR_NETWORK_TCP_IP_SERVER_IPP_INCLUDED
#define MSR_NETWORK_TCP_IP_SERVER_IPP_INCLUDED

namespace msr {
    namespace network {
        server<protocol::ip::tcp>::server(const protocol::ip::tcp::endpoint &endpoint):
            endpoint_(endpoint), acceptor_(service_, endpoint, true) {
            service_.stop();
        }
        void server<protocol::ip::tcp>::accept() {
            auto next_connection = std::make_shared<connection>(service_);
            acceptor_.async_accept(next_connection->socket_, [next_connection, this](error e) mutable {
                if (!e) {
                    connections_.push_back(next_connection);
                } else {
                    next_connection = nullptr;
                }
                broadcast(&observer::server_did_accept, next_connection, e);
            });
        }
        void server<protocol::ip::tcp>::send(connection_ptr c, data d) {
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
                broadcast(&observer::server_did_send, c, e, *data_);
            });
        }
        void server<protocol::ip::tcp>::receive(connection_ptr c, std::size_t size) {
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
                broadcast(&observer::server_did_receive, c, e, *data_);
            });
        }
        void server<protocol::ip::tcp>::cancel(connection_ptr c) {
            if (!c) {
                return;
            }
            error e;
            c->socket_.cancel(e);
            if (e) {
                connections_.remove(c);
            }
            service_.post([this, c, e]() {
                broadcast(&observer::server_did_cancel, c, e);
            });
        }
        void server<protocol::ip::tcp>::disconnect(connection_ptr c) {
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
                broadcast(&observer::server_did_disconnect, p, e);
            });
        }
        void server<protocol::ip::tcp>::run() {
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
            error e;
            if (!acceptor_.is_open()) {
                acceptor_.open(endpoint_.protocol(), e);
            }
            service_.post([this, e]() {
                broadcast(&observer::server_did_run, e);
            });
        }
        void server<protocol::ip::tcp>::shutdown() {
            using namespace boost::asio;
            using ip::tcp;
            for (auto &c : connections_) {
                c->socket_.shutdown(tcp::socket::shutdown_both);
            }
            connections_.clear();
            service_.stop();
            error e;
            acceptor_.close(e);
            pool_.post([this, e]() {
                broadcast(&observer::server_did_shutdown, e);
            });
        }
        bool server<protocol::ip::tcp>::is_running() const {
            return !service_.stopped();
        }
        bool server<protocol::ip::tcp>::is_shutdown() const {
            return service_.stopped();
        }
        server<protocol::ip::tcp>::~server() {
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

