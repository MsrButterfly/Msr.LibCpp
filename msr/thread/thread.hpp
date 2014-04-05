#ifndef MSR_THREAD_THREAD_HPP_INCLUDED
#define MSR_THREAD_THREAD_HPP_INCLUDED

#include <boost/thread.hpp>

namespace msr {
    using boost::thread;
    namespace this_thread = boost::this_thread;
}

#include <msr/thread/detail/debug.hpp>
#include <msr/thread/thread_pool.hpp>
#include <msr/thread/thread_pool_exception.hpp>

#endif
