#ifndef MSR_GL_DETAIL_SHADER_IPP_INCLUDED
#define MSR_GL_DETAIL_SHADER_IPP_INCLUDED

namespace msr {
    namespace gl {
        shader::shader()
        : shader(unknown_shader) {}
        shader::shader(const shader_type &shader_type_)
        : shader(shader_type_, "") {}
        shader::shader(const shader_type &shader_type_,
                       std::ifstream &&fin)
        : shader(shader_type_, "") {
            load(std::move(fin));
        }
        shader::shader(const shader_type &shader_type_,
                       const std::string &source)
        : shader_type_(shader_type_),
          source_(source),
          compiled_(false) {
            shader_ = glCreateShader(static_cast<GLenum>(shader_type_));
        }
        void shader::set_source(const std::string &source) {
            compiled_ = false;
            source_ = source;
        }
        const std::string &shader::source() const {
            return source_;
        }
        bool shader::load(const std::string &filename) {
            std::ifstream fin(filename);
            return load(std::move(fin));
        }
        bool shader::load(std::ifstream &&fin) {
            compiled_ = false;
            if (!fin.is_open()) {
                return false;
            }
            source_ = "";
            std::string line;
            while (std::getline(fin, line)) {
                source_ += line;
                source_ += '\n';
            }
            return true;
        }
        const shader::shader_type &shader::type() const {
            return shader_type_;
        }
        void shader::set_type(const shader_type &type) {
            if (type != shader_type_) {
                compiled_ = false;
                shader_type_ = type;
            }
        }
        bool shader::compile() {
            const char *code[] = {source_.c_str()};
            glShaderSource(shader_, 1, code, nullptr);
            glCompileShader(shader_);
            GLint result;
            glGetShaderiv(shader_, GL_COMPILE_STATUS, &result);
            compiled_ = (result == GL_TRUE);
            return compiled_;
        }
        const bool &shader::is_compiled() const {
            return compiled_;
        }
        std::string shader::last_compile_log() const {
            std::string last_compile_log_;
            GLint length;
            glGetShaderiv(shader_, GL_INFO_LOG_LENGTH, &length);
            char *log = new char[length]; // it also works when length == 0
            GLsizei written;
            glGetShaderInfoLog(shader_, length, &written, log);
            last_compile_log_ = log;
            delete[] log;
            return last_compile_log_;
        }
        shader::~shader() {
            glDeleteShader(shader_);
        }
    }
}

#endif

