#ifndef MSR_GL_UNIFORM_HPP_INCLUDED
#define MSR_GL_UNIFORM_HPP_INCLUDED

#include <string>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <msr/gl/program.hpp>

namespace msr {
    namespace gl {
        template <class T>
        class uniform {
        public:
            using self_type = uniform;
            using value_type = T;
            using handle_type = GLuint;
        public:
            uniform(program &program_, const std::string &name);
        public:
            self_type &operator=(const value_type &value);
            self_type &operator=(const self_type &another);
            value_type get() const;
            handle_type native_handle() const;
        private:
            program &program_;
            std::string name_;
        };
    }
}

#include <msr/gl/detail/uniform.ipp>
#include <msr/gl/detail/program.ipp>

#endif
