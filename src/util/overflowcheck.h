#ifndef __MINX_UTIL_OVERFLOWCHECK_H__
#define __MINX_UTIL_OVERFLOWCHECK_H__

#include <stdint.h>
#include <inttypes.h>

int minx_util_check_increment_overflow64(uint64_t);
int minx_util_check_decrement_overflow64(uint64_t);
int minx_util_check_addition_overflow64(uint64_t, uint64_t);
int minx_util_check_subtraction_overflow64(uint64_t, uint64_t);
int minx_util_check_multiplication_overflow64(uint64_t, uint64_t);
int minx_util_check_power_overflow64(uint64_t, uint64_t);

#endif //__MINX_UTIL_OVERFLOWCHECK_H__
