#ifndef MSR_THREAD_DETAIL_THREAD_POOL_EXCEPTION_INCLUDED
#define MSR_THREAD_DETAIL_THREAD_POOL_EXCEPTION_INCLUDED

#include <string>
#include <exception>
#include <msr/compatibility/exception.hpp>

namespace msr {
    class thread_pool_exception: public std::exception {
    public:
        using self = thread_pool_exception;
        using base = exception;
    public:
        thread_pool_exception():
            _what("thread_pool_exception: (null description)") {}
        thread_pool_exception(const std::string &what):
            _what("thread_pool_exception: " + what) {};
        thread_pool_exception(const self &e): _what(e.what()) {};
    public:
        self &operator=(const self &e) {
            _what = e.what();
            return *this;
        }
        virtual const char *what() const MSR_NOEXCEPT override{
            return _what.c_str();
        }
    private:
        std::string _what;
    };
}

#endif
