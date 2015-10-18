#ifndef MSR_THREAD_THREAD_POOL_HPP_INCLUDED
#define MSR_THREAD_THREAD_POOL_HPP_INCLUDED

#include <atomic>
#include <condition_variable>
#include <cstdlib>
#include <deque>
#include <exception>
#include <iostream>
#include <set>
#include <string>
#include <thread>
#include <boost/thread.hpp>
#include <boost/utility.hpp>
#include <msr/exception.hpp>
#include <msr/utility.hpp>

namespace msr {
    
    class thread_pool: boost::noncopyable {
        MSR_CLASS_TYPE_DEFINATIONS(thread_pool);
    public:
        using task = std::function<void ()>;
    private:
        using mutex = boost::shared_mutex;
        using readonly_lock = boost::shared_lock<mutex>;
        using readwrite_lock = boost::unique_lock<mutex>;
    public:
        thread_pool(const size_t size = std::thread::hardware_concurrency());
    public:
        void run();
        void stop();
        void post(task task_);
        void resize(const size_t size);
        void clear_task_queue();
        void wait() const;
        size_t size() const;
        bool stopped() const;
        bool task_queue_is_empty() const;
        bool this_thread_is_in() const;
    private:
        void task_loop();
        void unsafe_clear_task_queue();
        bool unsafe_task_queue_is_empty() const;
        bool unsafe_this_thread_is_in() const;
    public:
        virtual ~thread_pool();
    private:
        std::atomic<bool> loop_;
        std::atomic<bool> stopped_;
        std::atomic<size_t> size_;
        std::deque<task> task_queue_;
        std::set<std::unique_ptr<std::thread>> threads_;
        mutable mutex task_mutex_;
        mutable mutex thread_mutex_;
        mutable std::condition_variable_any task_queue_is_empty_;
        mutable std::condition_variable_any new_task_posted_;
    };
    
}

#include <msr/thread/detail/thread_pool.ipp>

#endif
