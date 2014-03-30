#ifndef MSR_NETWORK_DETAIL_OBSERVER_BASE_HPP_INCLUDED
#define MSR_NETWORK_DETAIL_OBSERVER_BASE_HPP_INCLUDED

#include <memory>
#include <boost/utility.hpp>

namespace msr {
    namespace network {
        namespace detail {
            class server_base;
            class observer_base: boost::noncopyable {
            public:
                using self = observer_base;
                using server = server_base;
            public:
                virtual ~observer_base() {}
            };
        }
    }
}

#endif
