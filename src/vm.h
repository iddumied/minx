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

#endif //__MINX_VM_H__
