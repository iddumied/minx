#include "util/overflowcheck.h"

int minx_util_check_increment_overflow64(uint64_t a) {
    return a == UINT64_MAX;
}

int minx_util_check_decrement_overflow64(uint64_t a) {
    return a == 0x0000;
}

int minx_util_check_addition_overflow64(uint64_t a, uint64_t b) {
    return (UINT64_MAX - a < b );
}

int minx_util_check_subtraction_overflow64(uint64_t a, uint64_t b) {
    return a < b;
}

int minx_util_check_multiplication_overflow64(uint64_t a, uint64_t b) {
    /* not implemented yet */
    return 1;
}

int minx_util_check_power_overflow64(uint64_t a, uint64_t b) {
    /* not implemented yet */
    return 1;
}
