#ifndef __MINX_KERNEL_H__
#define __MINX_KERNEL_H__

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

#include "util/print_prefixes.h"
#endif //if (defined DEBUG | defined VERBOSITY)

/*
 * And these headers are only required if compiled with DEBUG
 */
#ifdef DEBUG
#include <inttypes.h>

#include "util/debug.h"
#endif

/*
 * include headers for the kernel internals. They are outsourced because of
 * readability, grouping of same stuff and modularity
 */
#include "kernel/opcodes.h"
#include "kernel/heap.h"

#include "reader/binary_reader.h"

#include "util/error.h"
#include "util/config.h"

#include "def/sizes.h"
#include "def/statusbits.h"

#include "stack/stack.h"

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
 * Should be visible for all opcodes to read from it or write to it
 */
CommandParameters    *   opc_p           = NULL;
Stack                *   stack           = NULL;

/*
 * Function prototypes 
 */

void        minx_kernel_init                        (void);
int         minx_kernel_run                         (void);
void        minx_kernel_shutdown                    (void);

uint64_t    minx_kernel_program_pointer_get         (void);
void        minx_kernel_program_pointer_manipulate  (uint64_t new_pointer);
void        minx_kernel_unset_running_variable      (void);
void        minx_kernel_set_exit_status             (int);

extern void stack_print_binary                  (Stack*);

#endif //__MINX_KERNEL_H__
