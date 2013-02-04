#ifndef __MINX_VPU_STATUSBITS_H__
#define __MINX_VPU_STATUSBITS_H__

typedef enum {
    OVERFLOW_BIT = 0,
    ZERO_BIT,
    ALLOC_BIT,
    FREE_BIT,
    RESIZE_BIT,
    PUT_BIT,
    READ_BIT,
} StatusBits;

#endif // __MINX_VPU_STATUSBITS_H__
