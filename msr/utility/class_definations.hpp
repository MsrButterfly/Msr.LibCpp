#ifndef MSR_UTILITY_CLASS_DEFINATIONS_HPP_INCLUDED
#define MSR_UTILITY_CLASS_DEFINATIONS_HPP_INCLUDED

#include <memory>
#include <type_traits>

#define MSR_CLASS_TYPE_DEFINATIONS(self)               \
    public:                                            \
        using self_type = self;                        \
        using shared_ptr = std::shared_ptr<self_type>; \
        using weak_ptr = std::weak_ptr<self_type>;     \
        using unique_ptr = std::unique_ptr<self_type>;

#define MSR_CLASS_TYPE_DEFINATIONS_WITH_BASE(self, base) \
    MSR_CLASS_TYPE_DEFINATIONS(self);                    \
    public:                                              \
        using base_type = base;

#endif
