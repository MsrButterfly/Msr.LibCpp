#ifndef MSR_NETWORK_TCP_IP_SERVER_IPP_INCLUDED
#define MSR_NETWORK_TCP_IP_SERVER_IPP_INCLUDED

namespace msr {
    namespace network {
    
        tcp_ip_server::tcp_ip_server(const endpoint &p)
        : endpoint_(p), acceptor_(service_, p, true) {
            service_.stop();
        }
        
        void tcp_ip_server::accept() {
            auto c = std::make_shared<connection>(service_);
            acceptor_.async_accept(c->socket_, [c, this](error e) mutable {
                if (!e) {
                    connections_.insert(c);
                } else {
                    c = nullptr;
                }
                broadcast(&observer::server_did_accept, c, e);
            });
        }
        
        void tcp_ip_server::send(connection::shared_ptr c, data d) {
            using namespace boost::asio;
            assert(c);
            auto copy = std::make_shared<data>(d);
            async_write(c->socket_, copy->const_buffer(),
                [c, this, copy](error e, size_t size) {
                broadcast(&observer::server_did_send, c, e, *copy);
            });
        }
        
        void tcp_ip_server::receive(connection::shared_ptr c, size_t size) {
            using namespace boost::asio;
            assert(c);
            auto buffer = std::make_shared<data>(size);
            async_read(c->socket_, buffer->mutable_buffer(),
                [c, this, buffer](error e, size_t size) {
                broadcast(&observer::server_did_receive, c, e, *buffer);
            });
        }
        
#if BOOST_ASIO_ENABLE_CANCELIO
        void tcp_ip_server::cancel(connection::shared_ptr c) {
            assert(c);
            error e;
            c->socket_.cancel(e);
            service_.post([this, c, e]() {
                broadcast(&observer::server_did_cancel, c, e);
            });
        }
#endif

        void tcp_ip_server::disconnect(connection::shared_ptr c) {
            assert(c);
            error e;
            endpoint p;
            try {
                p = c->remote_endpoint();
            } catch (...) {}
            c->socket_.close();
            connections_.erase(c);
            service_.post([this, e, p]() {
                broadcast(&observer::server_did_disconnect, p, e);
            });
        }
        
        void tcp_ip_server::run() {
            using namespace boost::asio;
            if (!is_running()) {
                service_.reset();
                for (size_t i = 0; i < pool_.size(); ++i) {
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
        
        void tcp_ip_server::shutdown() {
            using namespace boost::asio;
            for (auto &c : connections_) {
                c->socket_.shutdown(ip::tcp::socket::shutdown_both);
            }
            service_.stop();
            connections_.clear();
            error e;
            acceptor_.close(e);
            pool_.post([this, e]() {
                broadcast(&observer::server_did_shutdown, e);
            });
        }
        
        bool tcp_ip_server::is_running() const {
            return !is_shutdown();
        }
        
        bool tcp_ip_server::is_shutdown() const {
            return service_.stopped();
        }
        
        tcp_ip_server::~tcp_ip_server() {
            if (is_running()) {
                shutdown();
            }
        }
        
    }
}

#endif
