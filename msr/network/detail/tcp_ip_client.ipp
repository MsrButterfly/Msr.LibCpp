#ifndef MSR_NETWORK_TCP_IP_CLIENT_IPP_INCLUDED
#define MSR_NETWORK_TCP_IP_CLIENT_IPP_INCLUDED

#include <cstdint>
#include <string>
#include <boost/lexical_cast.hpp>

namespace msr {
    namespace network {
    
        tcp_ip_client::tcp_ip_client() {
            service_.stop();
        }
        
        void tcp_ip_client::connect(const std::string &host, const uint16_t &port) {
            using namespace std;
            using namespace boost::asio;
            auto port_str = boost::lexical_cast<string>(port);
            auto c = make_shared<connection>(service_);
            ip::tcp::resolver resolver(service_);
            async_connect(c->socket_, resolver.resolve({host, port_str}),
                [c, this](error e, ip::tcp::resolver::iterator i) {
                connections_.insert(c);
                broadcast(&observer::client_did_connect, c, e);
            });
        }
        
        void tcp_ip_client::connect(const endpoint &p) {
            using namespace std;
            using namespace boost::asio;
            auto c = make_shared<connection>(service_);
            ip::tcp::resolver resolver(service_);
            async_connect(c->socket_, resolver.resolve(p),
                [c, this](error e, ip::tcp::resolver::iterator i) {
                connections_.insert(c);
                broadcast(&observer::client_did_connect, c, e);
            });
        }
        
        void tcp_ip_client::send(connection::shared_ptr c, data d) {
            using namespace std;
            using namespace boost::asio;
            assert(c);
            auto copy = make_shared<data>(d);
            async_write(c->socket_, copy->const_buffer(),
                [c, this, copy](error e, size_t size) {
                broadcast(&observer::client_did_send, c, e, *copy);
            });
        }
        
        void tcp_ip_client::receive(connection::shared_ptr c, size_t size) {
            using namespace std;
            using namespace boost::asio;
            assert(c);
            auto buffer = make_shared<data>(size);
            async_read(c->socket_, buffer->mutable_buffer(),
                [c, this, buffer](error e, size_t size) {
                broadcast(&observer::client_did_receive, c, e, *buffer);
            });
        }
        
#if BOOST_ASIO_ENABLE_CANCELIO
        void tcp_ip_client::cancel(connection::shared_ptr c) {
            assert(c);
            error e;
            c->socket_.cancel(e);
            service_.post([this, c, e]() {
                broadcast(&observer::client_did_cancel, c, e);
            });
        }
#endif

        void tcp_ip_client::disconnect(connection::shared_ptr c) {
            assert(c);
            error e;
            protocol::ip::tcp::endpoint p;
            try {
                p = c->remote_endpoint();
            } catch (...) {}
            c->socket_.close(e);
            connections_.erase(c);
            service_.post([this, e, p]() {
                broadcast(&observer::client_did_disconnect, p, e);
            });
        }
        
        void tcp_ip_client::run() {
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
            service_.post([this]() {
                broadcast(&observer::client_did_run);
            });
        }
        
        void tcp_ip_client::shutdown() {
            using namespace boost::asio;
            for (auto &c : connections_) {
                c->socket_.shutdown(ip::tcp::socket::shutdown_both);
            }
            service_.stop();
            connections_.clear();
            pool_.post([this]() {
                broadcast(&observer::client_did_shutdown);
            });
        }
        
        bool tcp_ip_client::is_running() const {
            return !is_shutdown();
        }
        
        bool tcp_ip_client::is_shutdown() const {
            return service_.stopped();
        }
        
        tcp_ip_client::~tcp_ip_client() {
            if (is_running()) {
                shutdown();
            }
        }
        
    }
}

#endif
