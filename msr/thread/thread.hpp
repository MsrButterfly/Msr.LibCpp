#ifndef MSR_THREAD_THREAD_HPP_INCLUDED
#define MSR_THREAD_THREAD_HPP_INCLUDED

#include <boost/thread.hpp>

namespace msr {
    namespace thread {
        using thread = boost::thread;
        namespace this_thread = boost::this_thread;
    }
}

#endif
