#ifndef __MINX_VPU_H__
#define __MINX_VPU_H__

/*
 * Standard includes, they are always required
 */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*
 * These headers are only required if compiled with DEBUG or VERBOSITY flag 
 */
#if (defined DEBUG | defined VERBOSITY)
#include <stdio.h>

#include "print_prefixes.h"
#endif //if (defined DEBUG | defined VERBOSITY)

/*
 * And these headers are only required if compiled with DEBUG
 */
#ifdef DEBUG
#include <inttypes.h>

#include "debug.h"
#endif

/*
 * include headers for the VPU internals. They are outsourced because of
 * readability, grouping of same stuff and modularity
 */
#include "opcodes.h"
#include "sizes.h"
#include "statusbits.h"
#include "heap.h"

#include "binary_reader.h"
#include "config.h"
#include "error.h"
#include "stack/stack.h"

/*
 * Register type
 */
typedef struct {
    uint64_t    value;
} Register;

/*
 * currently, registers are addressed with 1 byte addresses.
 * But the addresses are read from the binary as 2 byte, because if we want to
 * upgrade later, we do not have to change the compiler so much.
 */
#define         MAX_REGISTERS       (0x00FF+1) /* 256 */

#define         END_OF_PROGRAM      ((uint64_t)-1) /* last address is END_OF_PROGRAM */

#define         setbit(byte,bitnum)     do { byte |=  0x0001<<bitnum; } while(0)
#define         clrbit(byte,bitnum)     do { byte &= !(0x0001<<bitnum); } while(0)

/*
 * command parameters parsing helper struct
 */
typedef struct {
    unsigned int    len;
    uint64_t        p[];
} CommandParameters;

/*
 * Function prototypes 
 */

void    minx_vpu_init(void);
void    minx_vpu_run(void);
void    minx_vpu_shutdown(void);
extern void stack_print_binary(Stack*);

#endif //__MINX_VPU_H__
