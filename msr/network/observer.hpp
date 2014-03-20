#ifndef MSR_NETWORK_OBSERVER_HPP_INCLUDED
#define MSR_NETWORK_OBSERVER_HPP_INCLUDED

#include <memory>
#include <boost/utility.hpp>

namespace msr {
    namespace network {
        namespace detail {
            class observer_base: boost::noncopyable {
            public:
                using self = observer_base;
                using shared_ptr = std::shared_ptr<self>;
            };
        }
        template <class Server>
        class observer: public detail::observer_base {
        public:
            using self = observer;
            using base = observer_base;
            using server = Server;
            using shared_ptr = std::shared_ptr<self>;
        };
    }
}

#endif
