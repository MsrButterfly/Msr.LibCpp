#ifndef MSR_THREAD_THREAD_POOL_HPP_INCLUDED
#define MSR_THREAD_THREAD_POOL_HPP_INCLUDED

#include <atomic>
#include <condition_variable>
#include <deque>
#include <exception>
#include <iostream>
#include <set>
#include <string>
#include <thread>
#include <boost/thread.hpp>
#include <boost/utility.hpp>
#include <msr/thread/detail/debug.hpp>
#include <msr/thread/thread_pool_exception.hpp>
#include <msr/thread/thread.hpp>

namespace msr {
    class thread_pool: boost::noncopyable {
    public:
        using self = thread_pool;
        using thread = msr::thread;
        using task = std::function<void()>;
        using exception = thread_pool_exception;
    private:
        using mutex = boost::shared_mutex;
        using readonly_lock = boost::shared_lock<mutex>;
        using readwrite_lock = boost::unique_lock<mutex>;
    public:
        thread_pool(const std::size_t size = thread::hardware_concurrency()):
            loop_(true), stopped_(true), size_(size) {
            run();
        }
    public:
        void run() {
            readwrite_lock lock(thread_mutex_);
            if (!stopped()) {
                return;
            }
            loop_ = true;
            for (unsigned int i = 0; i < size_; i++) {
                threads_.insert(std::unique_ptr<thread>(new thread(&self::task_loop, this)));
            }
            stopped_ = false;
        }
        void stop() {
            readwrite_lock thread_lock(thread_mutex_);
            if (unsafe_this_thread_is_in()) {
                throw exception("stop(): Thread is trying to join itself.");
            }
            if (stopped()) {
                return;
            }
            {
                readwrite_lock task_lock(task_mutex_);
                loop_ = false;
                unsafe_clear_task_queue();
                new_task_posted_.notify_all();
            }
            for (auto &t : threads_) {
                try {
                    t->join();
                } catch (...) {
                    t->detach();
                }
            }
            threads_.clear();
            stopped_ = true;
        }
        bool stopped() const {
            return stopped_;
        }
        bool this_thread_is_in() const {
            readonly_lock lock(thread_mutex_);
            return unsafe_this_thread_is_in();
        }
        void post(task task_) {
            readwrite_lock lock(task_mutex_);
            task_queue_.push_back(task_);
            new_task_posted_.notify_one();
        }
        void wait() const {
            readonly_lock lock(task_mutex_);
            if (unsafe_this_thread_is_in()) {
                throw exception("wait(): Thread is trying to wait itself.");
            }
            if (!unsafe_task_queue_is_empty()) {
                task_queue_is_empty_.wait(lock);
            }
        }
        void clear_task_queue() {
            readwrite_lock lock(task_mutex_);
            unsafe_clear_task_queue();
            task_queue_is_empty_.notify_all();
        }
        bool task_queue_is_empty() const {
            readonly_lock lock(task_mutex_);
            return unsafe_task_queue_is_empty();
        }
        std::size_t size() const {
            return size_;
        }
        void resize(const std::size_t size) {
            if (!stopped()) {
                throw exception("resize(): Trying to resize a running thread pool.");
            }
            size_ = size;
        }
    private:
        void task_loop() {
            using namespace boost::chrono;
            using namespace this_thread;
            while (loop_) {
                task _task;
                {
                    readwrite_lock lock(task_mutex_);
                    /**
                     * 'while' and '_loop' are required!
                     * try to think about this scene.
                     * thread_a is waiting for a new task.
                     * then a new task comming, but thread_b
                     * is holding the lock, and thread_b
                     * finds that the task queue is not empty,
                     * so thread_b tries to get the task.
                     * meanwhile, thread_a is not blocking
                     * anymore, so thread_a is will also try
                     * to get the task. when one gets the task,
                     * another's action will cause a iterator
                     * reference error. so we use 'while' to
                     * prevent this situation.
                     */
                    while (unsafe_task_queue_is_empty() && loop_) {
#if MSR_THREAD_DEBUG(THREAD_POOL)
                        std::stringstream ss;
                        ss << "msr::thread::thread_pool: " << "[" << get_id() << ":wait]";
                        printf("%s\n", ss.str().c_str());
#endif
                        new_task_posted_.wait(lock);
                    }
                    if (!loop_) {
                        break;
                    }
                    _task = task_queue_.front();
                    task_queue_.pop_front();
                    if (unsafe_task_queue_is_empty()) {
                        task_queue_is_empty_.notify_all();
                    }
                }
#if MSR_THREAD_DEBUG(THREAD_POOL)
                auto begin = high_resolution_clock::now();
#endif
                _task();
#if MSR_THREAD_DEBUG(THREAD_POOL)
                auto end = high_resolution_clock::now();
                auto duration = end - begin;
                std::stringstream ss;
                ss << "msr::thread::thread_pool: " << "[" << get_id() << ":done] " << duration_cast<milliseconds>(duration).count() << "ms";
                printf("%s\n", ss.str().c_str());
#endif
            }
#if MSR_THREAD_DEBUG(THREAD_POOL)
            std::stringstream ss;
            ss << "msr::thread::thread_pool: " << "[" << get_id() << ":exit]";
            printf("%s\n", ss.str().c_str());
#endif
        }
        bool unsafe_this_thread_is_in() const {
            auto id = this_thread::get_id();
            for (const auto &t : threads_) {
                if (t->get_id() == id) {
                    return true;
                }
            }
            return false;
        }
        void unsafe_clear_task_queue() {
            task_queue_.clear();
        }
        bool unsafe_task_queue_is_empty() const {
            return task_queue_.empty();
        }
    public:
        virtual ~thread_pool() {
            if (!stopped()) {
                stop();
            }
        }
    private:
        std::atomic<bool> stopped_;
        std::atomic<bool> loop_;
        std::set<std::unique_ptr<thread>> threads_;
        std::deque<task> task_queue_;
        std::atomic<std::size_t> size_;
        mutable std::condition_variable_any new_task_posted_;
        mutable std::condition_variable_any task_queue_is_empty_;
        mutable mutex thread_mutex_;
        mutable mutex task_mutex_;
    };
}

#endif
