#ifndef MSR_MEMORY_ENDIAN_HPP_INCLUDED
#define MSR_MEMORY_ENDIAN_HPP_INCLUDED

#include <cstdint>

namespace msr {
    
    namespace detail {

        uint32_t endian_test_var = 0x01020304UL;

        constexpr bool is_big_endian() {
            return (uint8_t &)endian_test_var == 0x01;
        }

        constexpr bool is_little_endian() {
            return (uint8_t &)endian_test_var == 0x04;
        }

        constexpr bool is_pdp_endian() {
            return (uint8_t &)endian_test_var == 0x02;
        }

    }

    enum class endian {
        big_endian,
        little_endian,
        pdp_endian,
        unknown_endian
    };

    constexpr endian current_endian() {
        return detail::is_big_endian() ? endian::big_endian :
            detail::is_little_endian() ? endian::little_endian :
            detail::is_pdp_endian() ? endian::pdp_endian :
            endian::unknown_endian;
    }

}

#endif
