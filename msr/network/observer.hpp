#ifndef MSR_NETWORK_OBSERVER_HPP_INCLUDED
#define MSR_NETWORK_OBSERVER_HPP_INCLUDED

#include <boost/utility.hpp>
#include <msr/network/detail/observer_base.hpp>

namespace msr {
    namespace network {
        template <class T>
        struct is_observer: public boost::is_base_of<detail::observer_base, T> {};
        template <class X>
        class observer: public detail::observer_base {
        public:
            using self = observer;
            using base = observer_base;
        };
    }
}

#endif
