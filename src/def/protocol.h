#ifndef __MINX_PROTOCOL_H__
#define __MINX_PROTOCOL_H__

#include "def/sizes.h" /* for MAX_PARAMETER_COUNT */ 
#include "kernel/opcodes.h"

typedef struct {
    void (*opc_func)(uint64_t*);
    char *strrep;
    unsigned int params[MAX_PARAMETER_COUNT];
} OpcodeInformation;

extern const OpcodeInformation opcodes[];

#endif // __MINX_PROTOCOL_H__
