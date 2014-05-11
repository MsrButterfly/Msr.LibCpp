#ifndef MSR_GL_OUTPUT_HPP_INCLUDED
#define MSR_GL_OUTPUT_HPP_INCLUDED

#include <iostream>
#include <sstream>
#include <iomanip>
#include <glm/glm.hpp>

#define MSR_GL_VECTOR_STREAM_OUTPUT_FUNCTION(X)    \
template <class Char, class T, glm::precision P>   \
std::basic_ostream<Char> &operator<<(              \
  std::basic_ostream<Char> &os,                    \
  const glm::detail::tvec##X<T, P> &v) {           \
    os << '(';                                     \
    for (int i = 0; i < v.length(); i++) {         \
        os << v[i];                                \
        if (i != v.length() - 1) {                 \
            os << ',';                             \
        }                                          \
    }                                              \
    os << ')';                                     \
    return os;                                     \
}

#define MSR_GL_MATRIX_STREAM_OUTPUT_FUNCTION(X, Y) \
template <class Char, class T, glm::precision P>   \
std::basic_ostream<Char> &operator<<(              \
  std::basic_ostream<Char> &os,                    \
  const glm::detail::tmat##X##x##Y<T, P> &m) {     \
    using std::setw;                               \
    int width = 0;                                 \
    for (int i = 0; i < m.length(); i++) {         \
        for (int j = 0; j < m[i].length(); j++) {  \
            std::stringstream ss;                  \
            ss << m[i][j];                         \
            auto length = ss.str().length();       \
            if (width < length) {                  \
                width = static_cast<int>(length);  \
            }                                      \
        }                                          \
    }                                              \
    width++;                                       \
    for (int i = 0; i < m.length(); i++) {         \
        if (i == 0) {                              \
            os << "┌";                             \
        } else if (i == m.length() - 1) {          \
            os << "└";                             \
        } else {                                   \
            os << "│";                             \
        }                                          \
        for (int j = 0; j < m[i].length(); j++) {  \
            os << setw(width) << m[i][j];          \
        }                                          \
        if (i == 0) {                              \
            os << " ┐\n";                          \
        } else if (i == m.length() - 1) {          \
            os << " ┘";                            \
        } else {                                   \
            os << " │\n";                          \
        }                                          \
    }                                              \
    return os;                                     \
}

#ifndef MSR_GL_NO_STREAM_OUTPUT_FUNCTION

MSR_GL_VECTOR_STREAM_OUTPUT_FUNCTION(1)
MSR_GL_VECTOR_STREAM_OUTPUT_FUNCTION(2)
MSR_GL_VECTOR_STREAM_OUTPUT_FUNCTION(3)
MSR_GL_VECTOR_STREAM_OUTPUT_FUNCTION(4)

MSR_GL_MATRIX_STREAM_OUTPUT_FUNCTION(2, 2)
MSR_GL_MATRIX_STREAM_OUTPUT_FUNCTION(2, 3)
MSR_GL_MATRIX_STREAM_OUTPUT_FUNCTION(2, 4)
MSR_GL_MATRIX_STREAM_OUTPUT_FUNCTION(3, 2)
MSR_GL_MATRIX_STREAM_OUTPUT_FUNCTION(3, 3)
MSR_GL_MATRIX_STREAM_OUTPUT_FUNCTION(3, 4)
MSR_GL_MATRIX_STREAM_OUTPUT_FUNCTION(4, 2)
MSR_GL_MATRIX_STREAM_OUTPUT_FUNCTION(4, 3)
MSR_GL_MATRIX_STREAM_OUTPUT_FUNCTION(4, 4)

#endif

#endif
