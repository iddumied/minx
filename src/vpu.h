#ifndef __MINX_VPU_H__
#define __MINX_VPU_H__

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#if (defined DEBUG | defined VERBOSITY)
#include <stdio.h>

#include "minx.h"
#endif //if (defined DEBUG | defined VERBOSITY)

#ifdef DEBUG
#include <inttypes.h>

#include "debug.h"
#endif

#include "binary_reader.h"
#include "config.h"
#include "error.h"
#include "stack/stack.h"

/*
 * commands
 */

#define         OPC_NOP			    0x00
#define         OPC_CALL            0x01
#define         OPC_RET			    0x02

#define         OPC_MOV			    0x03
#define         OPC_MOVI		    0x04

#define         OPC_NOT			    0x05
#define         OPC_NOTR		    0x06

#define         OPC_AND			    0x07
#define         OPC_ANDI		    0x08
#define         OPC_ANDR		    0x09
#define         OPC_ANDIR		    0x0A

#define         OPC_OR			    0x0B
#define         OPC_ORI			    0x0C
#define         OPC_ORR			    0x0D
#define         OPC_ORIR		    0x0E

#define         OPC_DEC			    0x0F
#define         OPC_INC			    0x10

#define         OPC_LSH			    0x11
#define         OPC_RSH			    0x12

#define         OPC_CMP             0x13
#define         OPC_CMPI            0x14
#define         OPC_EQL             0x15
#define         OPC_EQLI            0x16

#define         OPC_PUSH		    0x20
#define         OPC_POP			    0x21
#define         OPC_DROP		    0x22

#define         OPC_ADD			    0x30
#define         OPC_ADDI		    0x31
#define         OPC_ADDR		    0x32
#define         OPC_ADDIR		    0x33

#define         OPC_JMP			    0x40
#define         OPC_JMPIZ		    0x41
#define         OPC_JMPNZ		    0x42
#define         OPC_IFZJMP		    0x43

#define         OPC_PSTACK          0x50
#define         OPC_PREGS           0x51
#define         OPC_PPROG           0x52

#define         OPC_ALLOC           0x60
#define         OPC_ALLOCI          0x61
#define         OPC_RESIZE          0x62
#define         OPC_RESIZEI         0x62
#define         OPC_FREE            0x63
#define         OPC_PUT             0x64
#define         OPC_READ            0x65
#define         OPC_GETSIZE         0x66

/* 
 * defines for size in bytes 
 */

#define         OPC_SIZE                2 /*16 bit*/
#define         VALUE_SIZE              8 /*64 bit*/
#define         REGISTER_ADDRESS_SIZE   2 /*16 bit*/
#define         HEAP_ADDRESS_SIZE       2 /*16 bit*/
#define         REGISTER_MASK           0x0000FFFF /* currently, only take 2 Byte */
#define         PROGRAM_ADDRESS_SIZE    8 /*64 bit, because program_pointer is 64 bit*/

typedef struct {
    uint64_t    value;
} Register;

typedef struct {
    uint64_t    reg_count;
    Register    *registers[];
} RegisterMap;

/*
 * currently, registers are addressed with 1 byte addresses.
 * But the addresses are read from the binary as 2 byte, because if we want to
 * upgrade later, we do not have to change the compiler so much.
 */
#define         MAX_REGISTERS       (0x00FF+1) /* 256 */

#define         END_OF_PROGRAM      ((uint64_t)-1) /* last address is END_OF_PROGRAM */

#define         OVERFLOW_BIT    0
#define         ALLOC_BIT       1

#define         setbit(byte,bitnum)     do { byte |=  0x0001<<bitnum; } while(0)
#define         clrbit(byte,bitnum)     do { byte &= !0x0001<<bitnum; } while(0)

/*
 * command parameters parsing helper struct
 */
typedef struct {
    unsigned int    len;
    uint64_t        p[];
} CommandParameters;

/*
 *
 * Heap 
 *
 * used: if this heapnode is used, this value is UINT8_MAX
 * when it is unused it is less than UINT8_MAX, and gets decremented each
 * VPU_MEM_CLEANUP times. If it is zero, it is removed from memory (free()).
 *
 * first_byte_addr: address of the first byte of this set of memory. Used for
 * the program.
 *
 * size: size of the memory. Used for both internals and the program.
 * *memory: pointer to the memory.
 */
typedef struct {
    uint8_t     used;
    uint64_t    first_byte_addr;
    uint64_t    size;
    uint64_t    real_size;
    void        *memory;
} HeapNode;

#define HEAPNODE_USED           UINT8_MAX
#define HEAPNODE_NOT_USED       0

/*
 * Function prototypes 
 */

void    minx_vpu_run(void);

#endif //__MINX_VPU_H__
