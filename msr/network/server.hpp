#ifndef MSR_NETWORK_SERVER_HPP_INCLUDED
#define MSR_NETWORK_SERVER_HPP_INCLUDED

#include <boost/utility.hpp>
#include <msr/network/detail/server_base.hpp>

namespace msr {
    namespace network {
        template <class T>
        struct is_server: public boost::is_base_of<detail::server_base, T> {};
        template <class Protocol>
        class server: public detail::server_base {
        public:
            using self = server;
            using base = server_base;
        };
    }
}

#endif
