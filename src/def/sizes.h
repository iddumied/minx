#ifndef __MINX_VPU_SIZES_H__
#define __MINX_VPU_SIZES_H__

/*
 * -----------------------------------------------------------------------------
 *
 *                              ! Protocol-Stable !
 *
 * -----------------------------------------------------------------------------
 *
 *                          DON'T CHANGE THESE SIZES, 
 *                          they are marked as stable in
 *                          v0.0.3-protocol-stable
 *
 * New sizes can be defined, but them in <v0.0.3-protocol-stable> shouldn't be
 * changed!
 */

/* 
 * <v0.0.3-protocol-stable> 
 */

#define         OPC_SIZE                2 /*16 bit*/
#define         VALUE_SIZE              8 /*64 bit*/
#define         REGISTER_ADDRESS_SIZE   2 /*16 bit*/
#define         HEAP_ADDRESS_SIZE       2 /*16 bit*/
#define         REGISTER_MASK           0x0000FFFF /* currently, only take 2 Byte */
#define         PROGRAM_ADDRESS_SIZE    8 /*64 bit, because program_pointer is 64 bit*/
#define         REGISTER_SIZE           VALUE_SIZE

#define         MAX_PARAMETER_COUNT     5

#define         END_OF_PROGRAM      ((uint64_t)-1) /* last address is END_OF_PROGRAM */

/* 
 * // <v0.0.3-protocol-stable> 
 */


/*
 * Print padding helpers
 */

#define         PROGRAM_POINTER_PADDING     "6"
#define         OPCODE_STRING_PADDING       "10"

/*
 * binary-loading sizes
 */
#define         MINX_ONE_CHUNK_LOAD_SIZE            (2 * MEBIBYTE)

/* max 10% of ram is for the binary of the running program. */
#define         MINX_BINARY_MAX_RAM_PERCENTAGE      10

/* 
 * min number of chunks in the memory.
 *
 * This is required because if you have 10G of RAM, it is not sure I can
 * allocate 1G in row. So I do some splitting into chunks.
 */
#define         MINX_BINARY_MIN_CHUNK_COUNT         10

#endif // __MINX_VPU_SIZES_H__
