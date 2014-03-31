#ifndef MSR_COMPATIBILITY_EXCEPTION_HPP_INCLUDED
#define MSR_COMPATIBILITY_EXCEPTION_HPP_INCLUDED

#ifndef __has_feature
#   define __has_feature(x) 0
#endif

#pragma warning(disable: 4067)
#if defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 180021114 || \
    defined(__GXX_EXPERIMENTAL_CXX0X__) && __GNUC__ * 10 + __GNUC_MINOR__ >= 46 || \
    defined(__clang__) && __has_feature(cxx_noexcept)
#  define MSR_NOEXCEPT noexcept
#else
#  define MSR_NOEXCEPT
#endif

#endif
