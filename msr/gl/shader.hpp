#ifndef MSR_GL_SHADER_HPP_INCLUDED
#define MSR_GL_SHADER_HPP_INCLUDED

#include <fstream>
#include <iostream>
#include <string>
#include <gl/glew.h>

namespace msr {
    namespace gl {
        class program;
        class shader {
            friend program;
        public:
            typedef enum {
                unknown_shader = 0,
                fragment_shader = GL_FRAGMENT_SHADER,
                vertex_shader = GL_VERTEX_SHADER
            } shader_type;
        public:
            shader();
            shader(const shader_type &shader_type_);
            shader(const shader_type &shader_type_,
                   std::ifstream &&fin);
            shader(const shader_type &shader_type_,
                   const std::string &source);
        public:
            const std::string &source() const;
            void set_source(const std::string &source);
            bool load(const std::string &filename);
            bool load(std::ifstream &&fin);
            const shader_type &type() const;
            void set_type(const shader_type &type);
            bool compile();
            const bool &is_compiled() const;
            std::string last_compile_log() const;
            const GLuint &native_handle() const;
        public:
            virtual ~shader();
        private:
            GLuint shader_;
            bool compiled_;
            std::string source_;
            shader_type shader_type_;
        };
    }
}

#include <msr/gl/detail/shader.ipp>

#endif
