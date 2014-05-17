#ifndef MSR_GL_DETAIL_PROGRAM_IPP_INCLUDED
#define MSR_GL_DETAIL_PROGRAM_IPP_INCLUDED

#include <boost/utility.hpp>

namespace msr {
    namespace gl {
        program::program() {
            program_ = glCreateProgram();
        }
        void program::attach(shader &shader_) {
            glAttachShader(program_, shader_.shader_);
        }
        void program::detach(shader &shader_) {
            glDetachShader(program_, shader_.shader_);
        }
        const bool &program::link(const std::vector<std::string> &ins,
                                  const std::vector<std::string> &outs) {
            for (unsigned int i = 0; i < ins.size(); i++) {
                glBindAttribLocation(program_, i, ins[i].c_str());
            }
            for (unsigned int i = 0; i < outs.size(); i++) {
                glBindFragDataLocation(program_, i, outs[i].c_str());
            }
            glLinkProgram(program_);
            GLint status;
            glGetProgramiv(program_, GL_LINK_STATUS, &status);
            linked_ = (status == GL_TRUE);
            return linked_;
        }
        const bool &program::is_linked() const {
            return linked_;
        }
        void program::use() {
            glUseProgram(program_);
        }
        std::string program::last_link_log() const {
            std::string last_link_log_;
            GLint length;
            glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &length);
            char *log = new char[length];
            GLsizei written;
            glGetProgramInfoLog(program_, length, &written, log);
            last_link_log_ = log;
            delete[] log;
            return last_link_log_;
        }
        const GLuint &program::native_handle() const {
            return program_;
        }
        program::~program() {
            glDeleteProgram(program_);
        }
        
#       define MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_NUM(glm_t, gl_t) \
        void program::set_uniform(const std::string &name,                \
                                  const glm::glm_t &n) {                  \
            auto location = glGetUniformLocation(program_, name.c_str()); \
            glUniform##gl_t(location, n);                                 \
        }
        
#       define MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_VEC(glm_t, gl_t) \
        void program::set_uniform(const std::string &name,                \
                                  const glm::glm_t &v) {                  \
            auto location = glGetUniformLocation(program_, name.c_str()); \
            glUniform##gl_t##v(location, 1, &v[0]);                       \
        }
        
#       define MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_MAT(glm_t, gl_t) \
        void program::set_uniform(const std::string &name,                \
                                  const glm::glm_t &m,                    \
                                  const bool &transpose) {                \
            auto location = glGetUniformLocation(program_, name.c_str()); \
            glUniformMatrix##gl_t##v(location, 1,                         \
                                     static_cast<GLboolean>(transpose),   \
                                     &m[0][0]);                           \
        }
        
        MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_NUM(float_t, 1f)
        MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_NUM(int_t, 1i)
        MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_NUM(uint_t, 1ui)
        
        MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_VEC(vec2, 2f)
        MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_VEC(vec3, 3f)
        MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_VEC(vec4, 4f)
        MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_VEC(ivec2, 2i)
        MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_VEC(ivec3, 3i)
        MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_VEC(ivec4, 4i)
        MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_VEC(uvec2, 2ui)
        MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_VEC(uvec3, 3ui)
        MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_VEC(uvec4, 4ui)
        MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_VEC(dvec2, 2d)
        MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_VEC(dvec3, 3d)
        MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_VEC(dvec4, 4d)
        
        MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_MAT(mat2, 2f)
        MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_MAT(mat2x3, 2x3f)
        MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_MAT(mat2x4, 2x4f)
        MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_MAT(mat3x2, 3x2f)
        MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_MAT(mat3, 3f)
        MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_MAT(mat3x4, 3x4f)
        MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_MAT(mat4x2, 4x2f)
        MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_MAT(mat4x3, 4x3f)
        MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_MAT(mat4, 4f)
        
#       undef MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_NUM
        
#       undef MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_VEC
        
#       undef MSR_GL_PROGRAM_SET_UNIFORM_IMPLEMENTATION_MAT
        
    }
}

#endif
