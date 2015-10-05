#ifndef MSR_UTILITY_CLASS_DEFINATIONS_HPP_INCLUDED
#define MSR_UTILITY_CLASS_DEFINATIONS_HPP_INCLUDED

#include <memory>
#include <type_traits>

#define MSR_CLASS_TYPE_DEFINATIONS(type)               \
    public:                                            \
        using self_type = type;                        \
        using shared_ptr = std::shared_ptr<self_type>; \
        using weak_ptr = std::weak_ptr<self_type>;     \
        using unique_ptr = std::unique_ptr<self_type>

#endif
