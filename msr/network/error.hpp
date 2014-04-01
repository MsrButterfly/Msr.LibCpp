#ifndef MSR_NETWORK_ERROR_HPP_INCLUDED
#define MSR_NETWORK_ERROR_HPP_INCLUDED

#include <boost/system/error_code.hpp>

namespace msr {
    namespace network {
        using error = boost::system::error_code;
    }
}

#endif

