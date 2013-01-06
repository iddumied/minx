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

#define         C_NOP			0x00
#define         C_RET			0x01

#define         C_MOV			0x02
#define         C_MOVI		    0x03

#define         C_NOT			0x04
#define         C_NOTR		    0x05

#define         C_AND			0x06
#define         C_ANDI		    0x07
#define         C_ANDR		    0x08
#define         C_ANDIR		    0x09

#define         C_OR			0x0A
#define         C_ORI			0x0B
#define         C_ORR			0x0C
#define         C_ORIR		    0x0D

#define         C_DEC			0x0E
#define         C_INC			0x0F

#define         C_LSH			0x10
#define         C_RSH			0x11

#define         C_PUSH		    0x20
#define         C_POP			0x21
#define         C_DROP		    0x22

#define         C_ADD			0x30
#define         C_ADDI		    0x31
#define         C_ADDR		    0x32
#define         C_ADDIR		    0x33

#define         C_JMP			0x40
#define         C_JMPIZ		    0x41
#define         C_JMPNZ		    0x42
#define         C_IFZJMP		0x43

/* 
 * defines for size in bytes 
 */

#define         COMMAND_SIZE    2
#define         VALUE_SIZE      8
#define         ADDRESS_SIZE    8

typedef struct {
    uint64_t *  value;
} Register;

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
