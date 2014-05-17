#ifndef MSR_GL_PROGRAM_HPP_INCLUDED
#define MSR_GL_PROGRAM_HPP_INCLUDED

#include <cstdint>
#include <vector>
#include <map>
#include <string>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <msr/gl/shader.hpp>

namespace msr {
    namespace gl {
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
            const handle_type &native_handle() const;
            void set_uniform(const std::string &name, const glm::float_t &f);
            void set_uniform(const std::string &name, const glm::int_t &i);
            void set_uniform(const std::string &name, const glm::uint_t &u);
            void set_uniform(const std::string &name, const glm::vec2 &v);
            void set_uniform(const std::string &name, const glm::vec3 &v);
            void set_uniform(const std::string &name, const glm::vec4 &v);
            void set_uniform(const std::string &name, const glm::ivec2 &v);
            void set_uniform(const std::string &name, const glm::ivec3 &v);
            void set_uniform(const std::string &name, const glm::ivec4 &v);
            void set_uniform(const std::string &name, const glm::uvec2 &v);
            void set_uniform(const std::string &name, const glm::uvec3 &v);
            void set_uniform(const std::string &name, const glm::uvec4 &v);
            void set_uniform(const std::string &name, const glm::dvec2 &v);
            void set_uniform(const std::string &name, const glm::dvec3 &v);
            void set_uniform(const std::string &name, const glm::dvec4 &v);
            void set_uniform(const std::string &name, const glm::mat2 &m,
                             const bool &transpose = false);
            void set_uniform(const std::string &name, const glm::mat2x3 &m,
                             const bool &transpose = false);
            void set_uniform(const std::string &name, const glm::mat2x4 &m,
                             const bool &transpose = false);
            void set_uniform(const std::string &name, const glm::mat3x2 &m,
                             const bool &transpose = false);
            void set_uniform(const std::string &name, const glm::mat3 &m,
                             const bool &transpose = false);
            void set_uniform(const std::string &name, const glm::mat3x4 &m,
                             const bool &transpose = false);
            void set_uniform(const std::string &name, const glm::mat4x2 &m,
                             const bool &transpose = false);
            void set_uniform(const std::string &name, const glm::mat4x3 &m,
                             const bool &transpose = false);
            void set_uniform(const std::string &name, const glm::mat4 &m,
                             const bool &transpose = false);
            void set_uniform(const std::string &name, const glm::dmat2 &m,
                             const bool &transpose = false);
            void set_uniform(const std::string &name, const glm::dmat2x3 &m,
                             const bool &transpose = false);
            void set_uniform(const std::string &name, const glm::dmat2x4 &m,
                             const bool &transpose = false);
            void set_uniform(const std::string &name, const glm::dmat3x2 &m,
                             const bool &transpose = false);
            void set_uniform(const std::string &name, const glm::dmat3 &m,
                             const bool &transpose = false);
            void set_uniform(const std::string &name, const glm::dmat3x4 &m,
                             const bool &transpose = false);
            void set_uniform(const std::string &name, const glm::dmat4x2 &m,
                             const bool &transpose = false);
            void set_uniform(const std::string &name, const glm::dmat4x3 &m,
                             const bool &transpose = false);
            void set_uniform(const std::string &name, const glm::dmat4 &m,
                             const bool &transpose = false);
        public:
            virtual ~program();
        private:
            GLuint program_;
            bool linked_;
        };
    }
}

#include <msr/gl/detail/program.ipp>

#endif

