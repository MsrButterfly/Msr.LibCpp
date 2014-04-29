#ifndef MSR_GL_PROGRAM_HPP_INCLUDED
#define MSR_GL_PROGRAM_HPP_INCLUDED

#include <cstdint>
#include <vector>
#include <map>
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
            const bool &link(const std::vector<std::string> &ins = {},
                             const std::vector<std::string> &outs = {});
            const bool &is_linked() const;
            void use();
            std::string last_link_log() const;
        public:
            virtual ~program();
        private:
            GLuint program_;
            bool linked_;
            std::map<std::string, unsigned int> ins_;
            std::map<std::string, unsigned int> outs_;
        };
    }
}

#include <msr/gl/detail/program.ipp>

#endif

