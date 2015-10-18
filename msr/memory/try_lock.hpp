#ifndef MSR_MEMORY_TRY_LOCK_HPP_INCLUDED
#define MSR_MEMORY_TRY_LOCK_HPP_INCLUDED

#include <memory>

namespace msr {

    template <class T>
    std::shared_ptr<T> try_lock(std::weak_ptr<T> p) {
        try {
            return p.lock();
        } catch (...) {
            return nullptr;
        }
    }
      
}

#endif
