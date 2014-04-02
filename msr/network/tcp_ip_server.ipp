#ifndef MSR_NETWORK_TCP_IP_SERVER_IPP_INCLUDED
#define MSR_NETWORK_TCP_IP_SERVER_IPP_INCLUDED

namespace msr {
    namespace network {
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
        void server<protocol::ip::tcp>::send(std::shared_ptr<connection> c, const data &d) {
            if (!c) {
                return;
            }
            auto data_ = std::make_shared<data>(d);
            boost::asio::async_write(c->socket_, data_->const_buffer(), [c, this, data_](error e, std::size_t size) {
                broadcast(&observer::server_did_send, c, e, *data_);
            });
        }
        void server<protocol::ip::tcp>::receive(std::shared_ptr<connection> c, const std::size_t &size) {
            if (!c) {
                return;
            }
            auto data_ = std::make_shared<data>(size);
            boost::asio::async_read(c->socket_, data_->mutable_buffer(), [c, this, data_](error e, std::size_t size) {
                broadcast(&observer::server_did_receive, c, e, *data_);
            });
        }
        void server<protocol::ip::tcp>::cancel(std::shared_ptr<connection> c) {
            error e;
            if (c) {
                c->socket_.cancel(e);
            }
            service_.post([this, c, e]() {
                broadcast(&observer::server_did_cancel, c, e);
            });
        }
        void server<protocol::ip::tcp>::disconnect(std::shared_ptr<connection> c) {
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
            service_.post([this, e, p]() {
                broadcast(&observer::server_did_disconnect, p, e);
            });
        }
        void server<protocol::ip::tcp>::run() {
            if (!is_running()) {
                service_.reset();
                for (std::size_t i = 0; i < pool_.size(); i++) {
                    pool_.post([this]() {
                        boost::asio::io_service::work work(service_);
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
            for (auto &c : connections_) {
                try {
                    c->socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
                } catch (...) {}
            }
            connections_.clear();
            if (!is_shutdown()) {
                service_.stop();
            }
            error e;
            if (acceptor_.is_open()) {
                acceptor_.close(e);
            }
            service_.post([this, e]() {
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
            try {
                shutdown();
            } catch (...) {}
        }
    }
}

#endif

