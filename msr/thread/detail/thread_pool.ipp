#ifndef MSR_THREAD_DETAIL_THREAD_POOL_IPP_INCLUDED
#define MSR_THREAD_DETAIL_THREAD_POOL_IPP_INCLUDED

#include <chrono>
#include <msr/thread/detail/debug.hpp>

namespace msr {

    thread_pool::thread_pool(const size_t size)
    : loop_(true), stopped_(true), size_(size) {
        run();
    }
    
    void thread_pool::run() {
        readwrite_lock lock(thread_mutex_);
        if (!stopped()) {
            return;
        }
        loop_ = true;
        for (size_t i = 0; i < size_; i++) {
            threads_.insert(std::make_unique<std::thread>(&self_type::task_loop, this));
        }
        stopped_ = false;
    }
    
    void thread_pool::stop() {
        readwrite_lock thread_lock(thread_mutex_);
        if (unsafe_this_thread_is_in()) {
            throw reverse_control(MSR_FUNCTION_SIGNATURE, "");
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
    
    bool thread_pool::stopped() const {
        return stopped_;
    }
    
    bool thread_pool::this_thread_is_in() const {
        readonly_lock lock(thread_mutex_);
        return unsafe_this_thread_is_in();
    }
    
    void thread_pool::post(task task_) {
        readwrite_lock lock(task_mutex_);
        task_queue_.push_back(task_);
        new_task_posted_.notify_one();
    }
    
    void thread_pool::wait() const {
        readonly_lock lock(task_mutex_);
        if (unsafe_this_thread_is_in()) {
            throw reverse_control(MSR_FUNCTION_SIGNATURE, "");
        }
        if (!unsafe_task_queue_is_empty()) {
            task_queue_is_empty_.wait(lock);
        }
    }
    
    void thread_pool::clear_task_queue() {
        readwrite_lock lock(task_mutex_);
        unsafe_clear_task_queue();
        task_queue_is_empty_.notify_all();
    }
    
    bool thread_pool::task_queue_is_empty() const {
        readonly_lock lock(task_mutex_);
        return unsafe_task_queue_is_empty();
    }
    
    size_t thread_pool::size() const {
        return size_;
    }
    
    void thread_pool::resize(const size_t size) {
        auto stopped_before = stopped();
        stop();
        size_ = size;
        if (!stopped_before) {
            run();
        }
    }
    
    void thread_pool::task_loop() {
        using namespace std::chrono;
        using namespace std::this_thread;
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
    
    bool thread_pool::unsafe_this_thread_is_in() const {
        auto id = std::this_thread::get_id();
        for (const auto &t : threads_) {
            if (t->get_id() == id) {
                return true;
            }
        }
        return false;
    }
    
    void thread_pool::unsafe_clear_task_queue() {
        task_queue_.clear();
    }
    
    bool thread_pool::unsafe_task_queue_is_empty() const {
        return task_queue_.empty();
    }
    
    thread_pool::~thread_pool() {
        stop();
    }
    
}

#endif

