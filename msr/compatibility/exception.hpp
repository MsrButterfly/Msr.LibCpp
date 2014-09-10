#ifndef MSR_COMPATIBILITY_EXCEPTION_HPP_INCLUDED
#define MSR_COMPATIBILITY_EXCEPTION_HPP_INCLUDED

#ifndef __has_feature
#    define __has_feature(x) 0
#endif

#if defined(__GXX_EXPERIMENTAL_CXX0X__) && __GNUC__ * 10 + __GNUC_MINOR__ >= 46 || \
    defined(__clang__) && __has_feature(cxx_noexcept)
#   define MSR_NOEXCEPT noexcept
#   define MSR_THROW(x) throw(x)
#elif defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 180021114
#   include <yvals.h>
#   define MSR_NOEXCEPT _NOEXCEPT
#   define MSR_THROW(x) throw(x)
#   pragma warning(disable: 4290)
#else
#   define MSR_NOEXCEPT
#   define MSR_THROW(x)
#endif

#endif
