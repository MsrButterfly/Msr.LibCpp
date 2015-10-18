#ifndef MSR_UTILITY_FUNCTION_NAME_HPP_INCLUDED
#define MSR_UTILITY_FUNCTION_NAME_HPP_INCLUDED

namespace msr {
    namespace detail {
        
        inline void function_name_helper() {
            
#if defined(__GNUC__) || defined(__ICC) && (__ICC >= 600)
#   define MSR_FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
#   define MSR_FUNCTION_SIGNATURE __FUNCSIG__
#else
#   define MSR_FUNCTION_SIGNATURE "(Unsupported)"
#endif

#if defined(__GNUC__) || defined(__ICC) && (__ICC >= 600)
#   define MSR_FUNCTION_NAME __func__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#   define MSR_FUNCTION_NAME __FUNC__
#elif defined(__FUNCTION__)
#   define MSR_FUNCTION_NAME __FUNCTION__
#else
#   define MSR_FUNCTION_NAME "(Unsupported)"
#endif

        }
        
    }
}

#endif
