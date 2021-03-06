#ifndef MSR_GL_DETAIL_PROGRAM_IPP_INCLUDED
#define MSR_GL_DETAIL_PROGRAM_IPP_INCLUDED

#include <boost/utility.hpp>

namespace msr {
    namespace gl {
        template <class T>
        uniform<T> program::uniform(const std::string &name) {
            return gl::uniform<T>(*this, name);
        }
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
    }
}

#endif
