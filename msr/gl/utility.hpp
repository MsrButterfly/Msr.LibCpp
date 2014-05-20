#ifndef MSR_GL_UTILITY_HPP_INCLUDED
#define MSR_GL_UTILITY_HPP_INCLUDED

#include <type_traits>
#include <glm/glm.hpp>

namespace msr {
    namespace gl {
        template <class T>
        struct is_val: std::false_type {};
        template <>
        struct is_val<glm::float_t>
        : std::true_type {};
        template <>
        struct is_val<glm::int_t>
        : std::true_type {};
        template <>
        struct is_val<glm::uint_t>
        : std::true_type {};
        template <class T>
        
        struct is_vec: std::false_type {};
        template <class T, glm::precision P>
        struct is_vec<glm::detail::tvec2<T, P>>
        : std::true_type {};
        template <class T, glm::precision P>
        struct is_vec<glm::detail::tvec3<T, P>>
        : std::true_type {};
        template <class T, glm::precision P>
        struct is_vec<glm::detail::tvec4<T, P>>
        : std::true_type {};
        
        template <class T>
        struct is_mat: std::false_type {};
        template <class T, glm::precision P>
        struct is_mat<glm::detail::tmat2x2<T, P>>
        : std::true_type {};
        template <class T, glm::precision P>
        struct is_mat<glm::detail::tmat2x3<T, P>>
        : std::true_type {};
        template <class T, glm::precision P>
        struct is_mat<glm::detail::tmat2x4<T, P>>
        : std::true_type {};
        template <class T, glm::precision P>
        struct is_mat<glm::detail::tmat3x2<T, P>>
        : std::true_type {};
        template <class T, glm::precision P>
        struct is_mat<glm::detail::tmat3x3<T, P>>
        : std::true_type {};
        template <class T, glm::precision P>
        struct is_mat<glm::detail::tmat3x4<T, P>>
        : std::true_type {};
        template <class T, glm::precision P>
        struct is_mat<glm::detail::tmat4x2<T, P>>
        : std::true_type {};
        template <class T, glm::precision P>
        struct is_mat<glm::detail::tmat4x3<T, P>>
        : std::true_type {};
        template <class T, glm::precision P>
        struct is_mat<glm::detail::tmat4x4<T, P>>
        : std::true_type {};
    }
}

#endif

