#ifndef MSR_UTILITY_OBSERVABLE_HPP_INCLUDED
#define MSR_UTILITY_OBSERVABLE_HPP_INCLUDED

#include <memory>
#include <set>
#include <type_traits>
#include <msr/utility/class_definations.hpp>

#define MSR_OBSERVER_TYPE(type) \
    public:                     \
        using observer = type

namespace msr {
        
    class observer {
        MSR_CLASS_TYPE_DEFINATIONS(observer);
    public:
        virtual ~observer() {};
    };

    template <class T>
    struct is_observer: public std::is_base_of<observer, T> {};

    class observable: public std::enable_shared_from_this<observable> {
        MSR_CLASS_TYPE_DEFINATIONS(observable);
        MSR_OBSERVER_TYPE(observer);
    public:
        void add_observer(observer::shared_ptr o) {
            observers_.insert(o);
        }
        void remove_observer(observer::shared_ptr o) {
            observers_.erase(o);
        }
    protected:
        template <class Observable, class Observer = typename Observable::observer>
        void broadcast(void (Observer:: *const f)(std::weak_ptr<Observable>)) {
            auto _this = std::weak_ptr<Observable>(std::static_pointer_cast<Observable>(shared_from_this()));
            for (auto &o : observers_) {
                if (auto _o = dynamic_cast<Observer *>(o.get())) {
                    _o.*f(_this);
                }
            }
        }
        template <class Observable, class ...Args, class Observer = typename Observable::observer>
        void broadcast(void (Observer:: *const f)(std::weak_ptr<Observable>, Args ...), const typename std::remove_cv<Args>::type &...args) {
            auto _this = std::weak_ptr<Observable>(std::static_pointer_cast<Observable>(shared_from_this()));
            for (auto &o : observers_) {
                if (auto _o = dynamic_cast<Observer *>(o.get())) {
                   _o.*f(_this, args...);
                }
            }
        }
    public:
        virtual ~observable() {};
    private:
        std::set<observer::shared_ptr> observers_;
    };
        
}

#endif
