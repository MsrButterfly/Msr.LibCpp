#ifndef MSR_MEMORY_BIT_HPP_INCLUDED
#define MSR_MEMORY_BIT_HPP_INCLUDED

#include <cstdlib>

namespace msr {

    template <size_t N>
    struct bit_size {
        static const size_t value = bit_size<(N >> 1)>::value + 1;
    };
    
    template <>
    struct bit_size<0> {
        static const size_t value = 0;
    };

}

#endif
