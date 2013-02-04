#if (defined DEBUG || defined DEBUGGING)

#ifndef __MINX_DEBUG_H__
#define __MINX_DEBUG_H__

#include <stdio.h>

#include "util/print_prefixes.h"

#define minxkerneldbgprintf(f,...)                                          \
    do {                                                                    \
        ConfigurationValue *cv = minx_config_get(CONF_MINX_DEBUGGING);      \
        if(cv != NULL && cv->b) {                                           \
            printf(MINX_KERNEL_PRINT_PREFIX": "f,__VA_ARGS__);              \
        }                                                                   \
    }while(0)

#define minxkerneldbgprint(f)                                               \
    do {                                                                    \
        ConfigurationValue *cv = minx_config_get(CONF_MINX_DEBUGGING);      \
        if(cv != NULL && cv->b) {                                           \
            printf(MINX_KERNEL_PRINT_PREFIX": "f);                          \
        }                                                                   \
    }while(0)

#define EXPLAIN_FUNCTION()                                                  \
    do{                                                                     \
        ConfigurationValue *cv = minx_config_get(CONF_SRC_DEBUGGING);       \
        if(cv != NULL && cv->b) {                                           \
            printf(MINX_KERNEL_FUNC_PRINT_PREFIX": %s, line %s", __func__,  \
                    __LINE__);                                              \
        }                                                                   \
    } while(0)

#define EXPLAIN_OPCODE(format,...)                                          \
    do {                                                                    \
        if(minx_config_get(CONF_SRC_DEBUGGING)->b) {                        \
            printf("("format")", __VA_ARGS__);                              \
        }                                                                   \
    } while(0)

#define EXPLAIN_OPCODE_WITH_HEXF(hexf,nonhexf,...)                          \
    do {                                                                    \
        if(minx_config_get(CONF_SRC_DEBUGGING)->b) {                        \
            if(minx_config_get(CONF_HEX)->b)                                \
                printf("("hexf")",__VA_ARGS__);                             \
            else                                                            \
                printf("("nonhexf")", __VA_ARGS__);                         \
        }                                                                   \
    } while(0)

#endif //__MINX_DEBUG_H__

#endif //DEBUG
