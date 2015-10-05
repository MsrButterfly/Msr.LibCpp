#ifndef MSR_THREAD_DETAIL_THREAD_POOL_EXCEPTION_INCLUDED
#define MSR_THREAD_DETAIL_THREAD_POOL_EXCEPTION_INCLUDED

#include <cstdint>
#include <msr/exception.hpp>

namespace msr {
    class thread_pool_exception: public exception {
    public:
        using self = thread_pool_exception;
        using base = exception;
        using id_t = std::uint8_t;
    public:
        thread_pool_exception(const id_t &id, const char *module, const char *description = "(undefined)")
        : base(0x10020000 + id, module, description) {}
    };
}

#endif
