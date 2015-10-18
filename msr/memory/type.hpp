#ifndef MSR_MEMORY_TYPE_HPP_INCLUDED
#define MSR_MEMORY_TYPE_HPP_INCLUDED

#include <climits>
#include <cstdint>
#include <type_traits>

#define MSR_BITS_PER_BYTE CHAR_BIT

#if defined(_WIN32) || defined(_WIN64)

#ifndef NOMINMAX
#   define NOMINMAX
#endif
#include <Windows.h>

namespace msr {

    using byte = ::BYTE;

    static const byte byte_all_zero = 0;
    static const byte byte_all_one = MAXBYTE;

    using word = ::WORD;

    static const word word_all_zero = 0;
    static const word word_all_one = MAXWORD;

}

#else

namespace msr {

    using byte = unsigned char;

    static const byte byte_all_zero = 0;
    static const byte byte_all_one = 0xff;

    using word = unsigned long;

    static const word word_all_zero = 0;
    static const word word_all_one = (word)-1;

}

#endif

namespace msr {

#if CHAR_BIT == 8

    using dual_byte = uint16_t;

    static const dual_byte dual_byte_all_zero = 0;
    static const dual_byte dual_byte_all_one = UINT16_MAX;

#else

    using dual_byte = byte[2];

    static const dual_byte dual_byte_all_zero = {0, 0};
    static const dual_byte dual_byte_all_one = {MAXBYTE, MAXBYTE};

#endif

}

#endif
