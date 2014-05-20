#ifndef MSR_GL_OUTPUT_HPP_INCLUDED
#define MSR_GL_OUTPUT_HPP_INCLUDED

#include <iomanip>
#include <iostream>
#include <sstream>
#include <glm/glm.hpp>
#include <msr/gl/utility.hpp>

#ifndef MSR_GL_NO_STREAM_OUTPUT_FUNCTION

template <class Char, class T>
typename std::enable_if<msr::gl::is_vec<T>::value,
std::basic_ostream<Char>>::type &operator<<(
  std::basic_ostream<Char> &os, const T &v) {
    os << '(';
    for (int i = 0; i < v.length(); i++) {
        os << v[i];
        if (i != v.length() - 1) {
            os << ',';
        }
    }
    os << ')';
    return os;
}

template <class Char, class T>
typename std::enable_if<msr::gl::is_mat<T>::value,
std::basic_ostream<Char>>::type &operator<<(
  std::basic_ostream<Char> &os, const T &m) {
    using std::setw;
    int width = 0;
    for (int i = 0; i < m.length(); i++) {
        for (int j = 0; j < m[i].length(); j++) {
            std::stringstream ss;
            ss << m[i][j];
            auto length = ss.str().length();
            if (width < length) {
                width = static_cast<int>(length);
            }
        }
    }
    width++;
    for (int i = 0; i < m.length(); i++) {
        if (i == 0) {
            os << "┌";
        } else if (i == m.length() - 1) {
            os << "└";
        } else {
            os << "│";
        }
        for (int j = 0; j < m[i].length(); j++) {
            os << setw(width) << m[i][j];
        }
        if (i == 0) {
            os << " ┐\n";
        } else if (i == m.length() - 1) {
            os << " ┘";
        } else {
            os << " │\n";
        }
    }
    return os;
}

#endif  // MSR_GL_NO_STREAM_OUTPUT_FUNCTION

#endif
