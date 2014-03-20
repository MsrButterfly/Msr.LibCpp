#ifndef MSR_NETWORK_SERVER_HPP_INCLUDED
#define MSR_NETWORK_SERVER_HPP_INCLUDED

#include <set>
#include <boost/asio.hpp>
#include <boost/utility.hpp>
#include <msr/network/observer.hpp>
#include <msr/thread.hpp>

namespace msr {
    namespace network {
        using service = boost::asio::io_service;
        using error = boost::system::error_code;
        enum class protocol {
            unknown = 0,
            tcp_ip = 1
        };
        namespace detail {
            class server_base: boost::noncopyable, std::enable_shared_from_this<server_base> {
            public:
                using self = server_base;
                using observer = network::observer<self>;
                using shared_ptr = std::shared_ptr<self>;
                using weak_ptr = std::weak_ptr<self>;
            public:
                server_base();
                server_base(self &&s) {}
            protected:
                template <class Observer, class ...Args>
                void broadcast(void (Observer:: *const f)(typename Observer::server::weak_ptr, Args...), const Args &...args) {
                    for (auto &o : _observers) {
                        if (Observer *p = dynamic_cast<Observer *>(o.get())) {
                            std::thread(f, p, typename Observer::server::weak_ptr(std::dynamic_pointer_cast<typename Observer::server, server_base>(shared_from_this())), args...).detach();
                        }
                    }
                }
            private:
                std::set<typename observer::shared_ptr> _observers;
                msr::thread::thread_pool _pool;
            };
        }
        template <protocol>
        class server: public detail::server_base {
        public:
            using self = server;
            using base = server_base;
            using observer = network::observer<self>;
            using shared_ptr = std::shared_ptr<self>;
            using weak_ptr = std::weak_ptr<self>;
        };
        template <protocol Protocol>
        class observer<server<Protocol>>: public detail::observer_base {
        public:
            using self = observer;
            using base = observer_base;
            using server = network::server<Protocol>;
            using shared_ptr = std::shared_ptr<self>;
        public:
            void test(typename server::weak_ptr s) {}
        };
        template <>
        class server<protocol::tcp_ip>: public detail::server_base {
        public:
            using self = server;
            using base = server_base;
            using observer = network::observer<self>;
            using shared_ptr = std::shared_ptr<self>;
            using weak_ptr = std::weak_ptr<self>;
            void _() {
                broadcast(&observer::test);
            }
        };
    }
}

#endif
