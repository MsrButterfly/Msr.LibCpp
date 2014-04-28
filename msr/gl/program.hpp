#ifndef MSR_GL_PROGRAM_HPP_INCLUDED
#define MSR_GL_PROGRAM_HPP_INCLUDED

#include <string>
#include <gl/glew.h>
#include <msr/gl/shader.hpp>

namespace msr {
    namespace gl {
        class program {
        public:
            program();
        public:
            void attach(shader &shader_);
            void detach(shader &shader_);
            const bool &link();
            const bool &is_linked() const;
            void use();
            const std::string &last_link_log() const;
        public:
            virtual ~program();
        private:
            GLuint program_;
            std::string last_link_log_;
            bool linked_;
        };
    }
}

#include <msr/gl/detail/program.ipp>

#endif

