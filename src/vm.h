#ifndef __MINX_VM_H__
#define __MINX_VM_H__

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "binary_reader.h"
#include "vm_commands.h"
#include "config.h"
#include "error.h"

#include "stack/stack.h"


/* defines for size in bytes */
#define COMMAND_SIZE    2
#define VALUE_SIZE      8
#define ADDRESS_SIZE    8

#define IN_BIT(bytes)   (bytes*8)

typedef struct {
    uint64_t * value;
} Register;

#define EXPLAIN_COMMAND(cmd,string) do{                                     \
                                        printf("minx command: %s\n\t%s\n",  \
                                                cmd, string );              \
                                    } while( 0 );


/* register aliases */

#define program_pointer (registers[0x0000].value)
#define akku            (registers[0x0002].value)
#define statusregister  (registers[0x0003].value)

typedef enum {
    OVERFLOW_BIT    =   0,
} StatusBit;

#define setbit(byte,bitnum)     do { byte |=  0x0001<<bitnum; } while(0)
#define clrbit(byte,bitnum)     do { byte &= !0x0001<<bitnum; } while(0)

/*
 * command parameters parsing helper struct
 */
typedef struct {
    uint64_t p1;
    uint64_t p2;
} CommandParameters;

#endif //__MINX_VM_H__
