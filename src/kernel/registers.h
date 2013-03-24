#ifndef __MINX_KERNEL_REGISTERS_H__
#define __MINX_KERNEL_REGISTERS_H__

#include <stdlib.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include "util/config.h"
#include "util/debug.h"
#include "util/error.h"

#include "stack/stack.h"

/* @typedef Register Register Type
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

void            minx_registers_init                 (void);
void            minx_registers_shutdown             (void);
uint16_t        minx_registers_get_register_count   (void);
Register*       minx_registers_find_register        (uint64_t addr);

int             minx_registers_push                 (void);
int             minx_registers_pop                  (void);

/*
 * optional prototypes
 */
#ifdef VERBOSITY
void            minx_registers_print_register   (unsigned int i);
#endif 

#define program_pointer_register_number 0x0000
#define akku_register_number            0x0002 
#define statusregister                  0x0003

#endif //__MINX_KERNEL_REGISTERS_H__

