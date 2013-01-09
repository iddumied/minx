#ifndef __MINX_VM_H__
#define __MINX_VM_H__

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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

/* 
 * defines for size in bytes 
 */

#define         OPC_SIZE        2
#define         VALUE_SIZE      8
#define         ADDRESS_SIZE    8

typedef struct {
    uint64_t    addr;
    uint64_t    value;
} Register;

typedef struct {
    uint64_t    reg_count;
    Register    *registers[];
} RegisterMap;

#define         DEFAULT_REGISTER_CNT            0xAF
#define         DEFAULT_ADDITIONAL_REGISTERS    0x0F

#define         OVERFLOW_BIT    0

#define         setbit(byte,bitnum)     do { byte |=  0x0001<<bitnum; } while(0)
#define         clrbit(byte,bitnum)     do { byte &= !0x0001<<bitnum; } while(0)

/*
 * command parameters parsing helper struct
 */
typedef struct {
    uint64_t    p1;
    uint64_t    p2;
} CommandParameters;

#endif //__MINX_VM_H__
