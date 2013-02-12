#ifndef __MINX_VPU_STATUSBITS_H__
#define __MINX_VPU_STATUSBITS_H__


/*
 * -----------------------------------------------------------------------------
 *
 *                              ! Protocol-Stable !
 *
 * -----------------------------------------------------------------------------
 *
 *                              DON'T CHANGE THIS ENUM, 
 *                              it is marked as stable in
 *                              v0.0.3-protocol-stable
 *
 * New entries can be defined, but only at the end of this enum, so the chain is
 * not changed!
 */

typedef enum {
    OVERFLOW_BIT = 0,
    ZERO_BIT,
    DIVZERO_BIT,
    ALLOC_BIT, /* set if alloc didn't work */
    FREE_BIT, /* set if free didn't work */
    RESIZE_BIT, /* set if resize didn't work */
    PUT_BIT, /* set if put didn't work */
    READ_BIT, /* set if read didn't work */
    NO_RAND, /* set if there is no random fetched */
} StatusBits;

#endif // __MINX_VPU_STATUSBITS_H__
