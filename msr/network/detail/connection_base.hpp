#ifndef MSR_NETWORK_DETAIL_CONNECTION_BASE_HPP_INCLUDED
#define MSR_NETWORK_DETAIL_CONNECTION_BASE_HPP_INCLUDED

#include <boost/utility.hpp>

namespace msr {
    namespace network {
        namespace detail {
            class connection_base: boost::noncopyable {
            public:
                using self = connection_base;
            public:
                virtual ~connection_base() {}
            };
        }
    }
}

#endif
