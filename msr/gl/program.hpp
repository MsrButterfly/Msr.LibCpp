#ifndef MSR_GL_PROGRAM_HPP_INCLUDED
#define MSR_GL_PROGRAM_HPP_INCLUDED

#include <string>
#include <vector>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <msr/gl/shader.hpp>

namespace msr {
    namespace gl {
        template <class T>
        class uniform;
        class program {
        public:
            using handle_type = GLuint;
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
            template <class T>
            uniform<T> uniform(const std::string &name);
            const handle_type &native_handle() const;
        public:
            virtual ~program();
        private:
            GLuint program_;
            bool linked_;
        };
    }
}

#include <msr/gl/uniform.hpp>

#endif

