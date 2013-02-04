#include "util/overflowcheck.h"

int minx_util_check_addition_overflow64(uint64_t a, uint64_t b) {
    return (UINT64_MAX - a < b );
}

int minx_util_check_subtraction_overflow64(uint64_t a, uint64_t b) {
    return a < b;
}

int minx_util_check_multiplication_overflow64(uint64_t a, uint64_t b) {
    if( a > b ) return minx_util_check_multiplication_overflow64(b, a);

    if( a > UINT32_MAX && b > 1 ) return 1; /* overflow */

    uint32_t c = b >> 32;
    uint32_t d = UINT32_MAX & b;
    uint64_t r = a * c;
    uint64_t s = a * d;

    if( s > UINT32_MAX ) return 1; /* overflow */

    s <<= 32;

    return 0; /* no overflow */
}

int minx_util_check_power_overflow64(uint64_t a, uint64_t b) {
    /* not implemented yet */
}
