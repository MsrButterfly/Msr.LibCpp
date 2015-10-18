#ifndef MSR_MEMORY_ENDIAN_HPP_INCLUDED
#define MSR_MEMORY_ENDIAN_HPP_INCLUDED

#include <cstdint>

namespace msr {

    // reinterpret_cast is not allowed in constexpr in C++11,
    // and I don't want to import Boost.Endian.
    
    namespace detail {
        union endian_test_t {
            uint32_t val;
            uint8_t bytes[4];
        } const endian_test_var = {0x01020304};
    }

    bool is_big_endian() {
        return detail::endian_test_var.bytes[0] == 0x01;
    }

    bool is_little_endian() {
        return detail::endian_test_var.bytes[0] == 0x04;
    }

    bool is_pdp_endian() {
        return detail::endian_test_var.bytes[0] == 0x02;
    }

    bool is_unknown_endian() {
        return !is_big_endian() && !is_little_endian() && !is_pdp_endian();
    }

    enum class endian {
        big_endian,
        little_endian,
        pdp_endian,
        unknown_endian
    };

    endian current_endian() {
        return is_big_endian() ? endian::big_endian :
            is_little_endian() ? endian::little_endian :
            is_pdp_endian() ? endian::pdp_endian :
            endian::unknown_endian;
    }

}

#endif
