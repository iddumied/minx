#ifndef __MINX_VPU_SIZES_H__
#define __MINX_VPU_SIZES_H__

#define         OPC_SIZE                2 /*16 bit*/
#define         VALUE_SIZE              8 /*64 bit*/
#define         REGISTER_ADDRESS_SIZE   2 /*16 bit*/
#define         HEAP_ADDRESS_SIZE       2 /*16 bit*/
#define         REGISTER_MASK           0x0000FFFF /* currently, only take 2 Byte */
#define         PROGRAM_ADDRESS_SIZE    8 /*64 bit, because program_pointer is 64 bit*/
#define         REGISTER_SIZE           VALUE_SIZE

#define         MAX_PARAMETER_COUNT     5

#define         END_OF_PROGRAM      ((uint64_t)-1) /* last address is END_OF_PROGRAM */

#endif // __MINX_VPU_SIZES_H__
