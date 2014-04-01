#ifndef MSR_NETWORK_TCP_IP_CONNECTION_HPP_INCLUDED
#define MSR_NETWORK_TCP_IP_CONNECTION_HPP_INCLUDED

#include <list>
#include <memory>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <msr/network/server.hpp>
#include <msr/network/protocol.hpp>
#include <msr/network/connection.hpp>
#include <msr/network/data.hpp>

namespace msr {
    namespace network {
        template <>
        class connection<protocol::ip::tcp>: public detail::connection_base {
            friend server<protocol::ip::tcp>;
        private:
            using mutex = boost::shared_mutex;
            using readonly_lock = boost::shared_lock<mutex>;
            using readwrite_lock = boost::unique_lock<mutex>;
        public:
            using self = connection;
            using base = connection_base;
        public:
            connection(boost::asio::io_service &io_service):
                socket_(io_service) {}
            protocol::ip::tcp::socket::endpoint_type remote_endpoint() const {
                return socket_.remote_endpoint();
            }
        private:
            template <class Value>
            std::shared_ptr<data> create_data(const Value &value) {
                readwrite_lock lock(data_mutex_);
                auto data_ = std::make_shared<data>(value);
                datas_.push_back(data_);
                return data_;
            }
            template <class Value>
            void remove_data(std::shared_ptr<data> data_) {
                readwrite_lock lock(data_mutex_);
                datas_.remove(data_);
            }
        private:
            protocol::ip::tcp::socket socket_;
            mutex data_mutex_;
            std::list<std::shared_ptr<data>> datas_;
        };
    }
}

#endif
