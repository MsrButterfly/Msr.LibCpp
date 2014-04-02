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
                broadcast(&observer::did_accept, next_connection, e);
            });
        }
        void server<protocol::ip::tcp>::send(std::shared_ptr<connection> c, const data &d) {
            if (!c) {
                throw exception("Null connection.");
            }
            auto data_ = std::make_shared<data>(d);
            boost::asio::async_write(c->socket_, data_->const_buffer(), [c, this, data_](error e, std::size_t size) {
                broadcast(&observer::did_send, c, e, *data_);
            });
        }
        void server<protocol::ip::tcp>::receive(std::shared_ptr<connection> c, const std::size_t &size) {
            if (!c) {
                throw exception("Null connection.");
            }
            auto data_ = std::make_shared<data>(size);
            boost::asio::async_read(c->socket_, data_->mutable_buffer(), [c, this, data_](error e, std::size_t size) {
                broadcast(&observer::did_receive, c, e, *data_);
            });
        }
        void server<protocol::ip::tcp>::cancel(std::shared_ptr<connection> c) {
            if (!c) {
                throw exception("Null connection.");
            }
            c->socket_.cancel();
        }
        void server<protocol::ip::tcp>::close(std::shared_ptr<connection> c) {
            if (!c) {
                throw exception("Null connection.");
            }
            c->socket_.close();
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
            broadcast(&observer::did_run, e);
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
            broadcast(&observer::did_shutdown, e);
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

