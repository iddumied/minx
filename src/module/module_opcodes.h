#ifndef __MINX_MODULE_OPCODES_H__
#define __MINX_MODULE_OPCODES_H__

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdint.h>

typedef struct {
    unsigned int    len;
    uint64_t        m_opcodes[];
} MOpcodes;

#endif // __MINX_MODULE_OPCODES_H__
