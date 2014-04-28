#ifndef MSR_GL_DETAIL_PROGRAM_IPP_INCLUDED
#define MSR_GL_DETAIL_PROGRAM_IPP_INCLUDED

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
        const bool &program::link() {
            glLinkProgram(program_);
            GLint status;
            glGetProgramiv(program_, GL_LINK_STATUS, &status);
            GLint length;
            glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &length);
            char *log = new char[length];
            GLsizei written;
            glGetProgramInfoLog(program_, length, &written, log);
            last_link_log_ = log;
            delete[] log;
            linked_ = (status == GL_TRUE);
            return linked_;
        }
        const bool &program::is_linked() const {
            return linked_;
        }
        void program::use() {
            glUseProgram(program_);
        }
        const std::string &program::last_link_log() const {
            return last_link_log_;
        }
        program::~program() {
            glDeleteProgram(program_);
        }
    }
}

#endif
