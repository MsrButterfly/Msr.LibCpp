#ifndef MSR_NETWORK_DETAIL_SERVER_BASE_HPP_INCLUDED
#define MSR_NETWORK_DETAIL_SERVER_BASE_HPP_INCLUDED

#include <list>
#include <memory>
#include <boost/utility.hpp>
#include <msr/network/detail/connection_base.hpp>
#include <msr/network/server_exception.hpp>
#include <msr/network/server_observer.hpp>
#include <msr/thread.hpp>
#include <boost/signals2.hpp>

namespace msr {
    namespace network {
        namespace detail {
            class server_base: boost::noncopyable, public std::enable_shared_from_this<server_base> {
            public:
                using self = server_base;
                using connection = connection_base;
                using observer = observer_base;
                using exception = server_exception;
            public:
                server_base() {}
            public:
                template <class Observer, class ...Args>
                typename boost::enable_if<is_observer<Observer>,
                std::shared_ptr<Observer>>::type create_observer(Args &&...args) {
                    auto o = std::make_shared<Observer>(std::forward<Args>(args)...);
                    observers_.insert(o);
                    return o;
                }
            protected:
                template <class Server, class ...Args>
                void broadcast(void (network::observer<Server>:: *const f)(std::weak_ptr<Server>, typename std::remove_reference<Args>::type ...),
                               Args &&...args) {
                    auto _this = std::weak_ptr<Server>(std::dynamic_pointer_cast<Server>(shared_from_this()));
                    boost::signals2::signal<void ()> signal;
                    for (auto &o : observers_) {
                        if (auto _o = dynamic_cast<network::observer<Server> *>(o.get())) {
                            signal.connect(boost::bind(f, _o, _this, std::forward<Args>(args)...));
                        }
                    }
                    signal();
                }
            public:
                virtual ~server_base() {}
            protected:
                std::list<std::shared_ptr<connection>> connections_;
            private:
                std::set<std::shared_ptr<observer>> observers_;
            };
        }
    }
}
#endif
