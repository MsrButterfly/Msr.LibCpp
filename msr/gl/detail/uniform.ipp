#ifndef MSR_GL_DETAIL_UNIFORM_IPP_INCLUDED
#define MSR_GL_DETAIL_UNIFORM_IPP_INCLUDED

#include <utility>
#include <glm/glm.hpp>
#include <msr/gl/utility.hpp>

namespace msr {
    namespace gl {
        template <class T> typename uniform<T>::handle_type
        uniform<T>::native_handle() const {
            return glGetUniformLocation(program_.native_handle(),
                                        name_.c_str());
        }
        template <class T>
        uniform<T>::uniform(program &program_, const std::string &name)
        : program_(program_), name_(name) {}
        template <class T>
        uniform<T> &uniform<T>::operator=(const self_type &another) {
            operator=(another.get());
        }
#define MSR_GL_UNIFORM_VAL_ASSIGN_IMPLEMENTATION(glm_t, gl_t)      \
        template <> uniform<glm::glm_t> &                          \
        uniform<glm::glm_t>::operator=(const glm::glm_t &value) {  \
            glUniform##gl_t(native_handle(), value);               \
            return *this;                                          \
        }
        MSR_GL_UNIFORM_VAL_ASSIGN_IMPLEMENTATION(float_t, 1f)
        MSR_GL_UNIFORM_VAL_ASSIGN_IMPLEMENTATION(int_t, 1i)
        MSR_GL_UNIFORM_VAL_ASSIGN_IMPLEMENTATION(uint_t, 1i)
#undef  MSR_GL_UNIFORM_VAL_ASSIGN_IMPLEMENTATION
#define MSR_GL_UNIFORM_VEC_ASSIGN_IMPLEMENTATION(glm_t, gl_t)      \
        template <> uniform<glm::glm_t> &                          \
        uniform<glm::glm_t>::operator=(const glm::glm_t &value) {  \
            glUniform##gl_t##v(native_handle(), 1, &value[0]);     \
            return *this;                                          \
        }
        MSR_GL_UNIFORM_VEC_ASSIGN_IMPLEMENTATION(vec2, 2f)
        MSR_GL_UNIFORM_VEC_ASSIGN_IMPLEMENTATION(vec3, 3f)
        MSR_GL_UNIFORM_VEC_ASSIGN_IMPLEMENTATION(vec4, 4f)
        MSR_GL_UNIFORM_VEC_ASSIGN_IMPLEMENTATION(ivec2, 2i)
        MSR_GL_UNIFORM_VEC_ASSIGN_IMPLEMENTATION(ivec3, 3i)
        MSR_GL_UNIFORM_VEC_ASSIGN_IMPLEMENTATION(ivec4, 4i)
        MSR_GL_UNIFORM_VEC_ASSIGN_IMPLEMENTATION(uvec2, 2ui)
        MSR_GL_UNIFORM_VEC_ASSIGN_IMPLEMENTATION(uvec3, 3ui)
        MSR_GL_UNIFORM_VEC_ASSIGN_IMPLEMENTATION(uvec4, 4ui)
        MSR_GL_UNIFORM_VEC_ASSIGN_IMPLEMENTATION(dvec2, 2d)
        MSR_GL_UNIFORM_VEC_ASSIGN_IMPLEMENTATION(dvec3, 3d)
        MSR_GL_UNIFORM_VEC_ASSIGN_IMPLEMENTATION(dvec4, 4d)
#undef  MSR_GL_UNIFORM_VEC_ASSIGN_IMPLEMENTATION
#define MSR_GL_UNIFORM_MAT_ASSIGN_IMPLEMENTATION(glm_t, gl_t)      \
        template <> uniform<glm::glm_t> &                          \
        uniform<glm::glm_t>::operator=(const glm::glm_t &value) {  \
            glUniformMatrix##gl_t##v(native_handle(), 1, false,    \
                                     &value[0][0]);                \
            return *this;                                          \
        }
        MSR_GL_UNIFORM_MAT_ASSIGN_IMPLEMENTATION(mat2, 2f)
        MSR_GL_UNIFORM_MAT_ASSIGN_IMPLEMENTATION(mat2x3, 2x3f)
        MSR_GL_UNIFORM_MAT_ASSIGN_IMPLEMENTATION(mat2x4, 2x4f)
        MSR_GL_UNIFORM_MAT_ASSIGN_IMPLEMENTATION(mat3x2, 3x2f)
        MSR_GL_UNIFORM_MAT_ASSIGN_IMPLEMENTATION(mat3, 3f)
        MSR_GL_UNIFORM_MAT_ASSIGN_IMPLEMENTATION(mat3x4, 3x4f)
        MSR_GL_UNIFORM_MAT_ASSIGN_IMPLEMENTATION(mat4x2, 4x2f)
        MSR_GL_UNIFORM_MAT_ASSIGN_IMPLEMENTATION(mat4x3, 4x3f)
        MSR_GL_UNIFORM_MAT_ASSIGN_IMPLEMENTATION(mat4, 4f)
        MSR_GL_UNIFORM_MAT_ASSIGN_IMPLEMENTATION(dmat2, 2d)
        MSR_GL_UNIFORM_MAT_ASSIGN_IMPLEMENTATION(dmat2x3, 2x3d)
        MSR_GL_UNIFORM_MAT_ASSIGN_IMPLEMENTATION(dmat2x4, 2x4d)
        MSR_GL_UNIFORM_MAT_ASSIGN_IMPLEMENTATION(dmat3x2, 3x2d)
        MSR_GL_UNIFORM_MAT_ASSIGN_IMPLEMENTATION(dmat3, 3d)
        MSR_GL_UNIFORM_MAT_ASSIGN_IMPLEMENTATION(dmat3x4, 3x4d)
        MSR_GL_UNIFORM_MAT_ASSIGN_IMPLEMENTATION(dmat4x2, 4x2d)
        MSR_GL_UNIFORM_MAT_ASSIGN_IMPLEMENTATION(dmat4x3, 4x3d)
        MSR_GL_UNIFORM_MAT_ASSIGN_IMPLEMENTATION(dmat4, 4d)
#undef  MSR_GL_UNIFORM_MAT_ASSIGN_IMPLEMENTATION
#define MSR_GL_UNIFORM_VAL_GET_IMPLEMENTATION(glm_t, gl_t)         \
        template <> uniform<glm::glm_t>::value_type                \
        uniform<glm::glm_t>::get() const {                         \
            value_type value;                                      \
            glGetUniform##gl_t##v(program_.native_handle(),        \
                                  native_handle(), &value);        \
            return value;                                          \
        }
        MSR_GL_UNIFORM_VAL_GET_IMPLEMENTATION(float_t, f)
        MSR_GL_UNIFORM_VAL_GET_IMPLEMENTATION(int_t, i)
        MSR_GL_UNIFORM_VAL_GET_IMPLEMENTATION(uint_t, ui)
#undef  MSR_GL_UNIFORM_VAL_GET_IMPLEMENTATION
#define MSR_GL_UNIFORM_VEC_GET_IMPLEMENTATION(glm_t, gl_t)         \
        template <> uniform<glm::glm_t>::value_type                \
        uniform<glm::glm_t>::get() const {                         \
            value_type value;                                      \
            glGetUniform##gl_t##v(program_.native_handle(),        \
                                  native_handle(), &value[0]);     \
            return value;                                          \
        }
        MSR_GL_UNIFORM_VEC_GET_IMPLEMENTATION(vec2, f)
        MSR_GL_UNIFORM_VEC_GET_IMPLEMENTATION(vec3, f)
        MSR_GL_UNIFORM_VEC_GET_IMPLEMENTATION(vec4, f)
        MSR_GL_UNIFORM_VEC_GET_IMPLEMENTATION(ivec2, i)
        MSR_GL_UNIFORM_VEC_GET_IMPLEMENTATION(ivec3, i)
        MSR_GL_UNIFORM_VEC_GET_IMPLEMENTATION(ivec4, i)
        MSR_GL_UNIFORM_VEC_GET_IMPLEMENTATION(uvec2, ui)
        MSR_GL_UNIFORM_VEC_GET_IMPLEMENTATION(uvec3, ui)
        MSR_GL_UNIFORM_VEC_GET_IMPLEMENTATION(uvec4, ui)
        MSR_GL_UNIFORM_VEC_GET_IMPLEMENTATION(dvec2, d)
        MSR_GL_UNIFORM_VEC_GET_IMPLEMENTATION(dvec3, d)
        MSR_GL_UNIFORM_VEC_GET_IMPLEMENTATION(dvec4, d)
#undef  MSR_GL_UNIFORM_VEC_GET_IMPLEMENTATION
#define MSR_GL_UNIFORM_MAT_GET_IMPLEMENTATION(glm_t, gl_t)         \
        template <> uniform<glm::glm_t>::value_type                \
        uniform<glm::glm_t>::get() const {                         \
            value_type value;                                      \
            glGetUniform##gl_t##v(program_.native_handle(),        \
                                  native_handle(), &value[0][0]);  \
            return value;                                          \
        }
        MSR_GL_UNIFORM_MAT_GET_IMPLEMENTATION(mat2, f)
        MSR_GL_UNIFORM_MAT_GET_IMPLEMENTATION(mat2x3, f)
        MSR_GL_UNIFORM_MAT_GET_IMPLEMENTATION(mat2x4, f)
        MSR_GL_UNIFORM_MAT_GET_IMPLEMENTATION(mat3x2, f)
        MSR_GL_UNIFORM_MAT_GET_IMPLEMENTATION(mat3, f)
        MSR_GL_UNIFORM_MAT_GET_IMPLEMENTATION(mat3x4, f)
        MSR_GL_UNIFORM_MAT_GET_IMPLEMENTATION(mat4x2, f)
        MSR_GL_UNIFORM_MAT_GET_IMPLEMENTATION(mat4x3, f)
        MSR_GL_UNIFORM_MAT_GET_IMPLEMENTATION(mat4, f)
        MSR_GL_UNIFORM_MAT_GET_IMPLEMENTATION(dmat2, d)
        MSR_GL_UNIFORM_MAT_GET_IMPLEMENTATION(dmat2x3, d)
        MSR_GL_UNIFORM_MAT_GET_IMPLEMENTATION(dmat2x4, d)
        MSR_GL_UNIFORM_MAT_GET_IMPLEMENTATION(dmat3x2, d)
        MSR_GL_UNIFORM_MAT_GET_IMPLEMENTATION(dmat3, d)
        MSR_GL_UNIFORM_MAT_GET_IMPLEMENTATION(dmat3x4, d)
        MSR_GL_UNIFORM_MAT_GET_IMPLEMENTATION(dmat4x2, d)
        MSR_GL_UNIFORM_MAT_GET_IMPLEMENTATION(dmat4x3, d)
        MSR_GL_UNIFORM_MAT_GET_IMPLEMENTATION(dmat4, d)
#undef  MSR_GL_UNIFORM_MAT_GET_IMPLEMENTATION
    }
}

#endif

