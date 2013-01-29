#ifndef __MINX_VPU_REGISTERS_H__
#define __MINX_VPU_REGISTERS_H__

#include <stdlib.h>
#include <inttypes.h>

#include "util/debug.h"
#include "util/error.h"

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

void            minx_registers_init             (void);
void            minx_registers_shutdown         (void);
Register*       minx_registers_find_register    (uint64_t addr);

/*
 * optional prototypes
 */
#ifdef VERBOSITY
void            minx_registers_print_register   (unsigned int i);
#endif 

#define program_pointer_register_number 0x0000
#define akku_register_number            0x0002 
#define statusregister                  0x0003

#endif //__MINX_VPU_REGISTERS_H__

