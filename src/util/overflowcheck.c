#include "util/overflowcheck.h"

/**
 * @brief Check if the passed value will overflow if it gets incremented
 *
 * @param a The value
 *
 * @return true on overflow, else false
 */
int minx_util_check_increment_overflow64(uint64_t a) {
    return a == UINT64_MAX;
}

/**
 * @brief Check if the passed value will overflow if it gets decremented
 *
 * @param a The value
 *
 * @return true on overflow, else false
 */
int minx_util_check_decrement_overflow64(uint64_t a) {
    return a == 0x0000;
}

/**
 * @brief Check if the result of a addition of the passed values will overflow
 *
 * @param a The first addend
 * @param b The second addend
 *
 * @return true on overflow, else false
 */
int minx_util_check_addition_overflow64(uint64_t a, uint64_t b) {
    return (UINT64_MAX - a < b );
}

/**
 * @brief Check if the result of a subtraction of the passed values will overflow
 *
 * @param a Minuend
 * @param b Subtrahend
 *
 * @return true on overflow, else false
 */
int minx_util_check_subtraction_overflow64(uint64_t a, uint64_t b) {
    return a < b;
}

/**
 * @brief Check if the result of a multiplication of the passed values will overflow
 *
 *  u = UINT64_MAX
 *  b = b
 *  a = a
 *
 *  u / b < a    |*b
 *  u < a*b
 *
 *  if this is true, overflow will occour
 *
 * @param a Multiplier
 * @param b Multiplicand
 *
 * @return true on overflow, else false
 */
int minx_util_check_multiplication_overflow64(uint64_t a, uint64_t b) {
    return (UINT64_MAX / b) < a;
}

/**
 * @brief Check if raising a to the power of b will overflow
 *
 * Condition:
 *
 *  a^b > UINT64_MAX
 *
 * Mathematical calculation:
 *
 *  u = UINT64_MAX
 *  u > a
 *  u > b
 *
 *  u < a^b ?
 *
 *  2u > a + b
 *  u > (a + b) / 2
 *
 *  (a + b) / 2 < u < a^b
 *
 *
 * Don't know if this is true. But I think. Corrections welcome!
 *
 * @param a Basis
 * @param b Exponent
 *
 * @return true on overflow, else false
 */
int minx_util_check_power_overflow64(uint64_t a, uint64_t b) {
    return minx_util_check_addition_overflow64(a, b) || (a + b) / 2 < UINT64_MAX;
}
