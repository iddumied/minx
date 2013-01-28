#if (defined DEBUG || defined DEBUGGING)

#ifndef __MINX_DEBUG_H__
#define __MINX_DEBUG_H__

#include <stdio.h>

#include "print_prefixes.h"

#define minxvpudbgprintf(f,...)                                             \
    do {                                                                    \
        ConfigurationValue *cv = minx_config_get(CONF_MVPU_DEBUGGING);      \
        if(cv != NULL && cv->b) {                                           \
            printf(MINX_VPU_PRINT_PREFIX": "f,__VA_ARGS__);                 \
        }                                                                   \
    }while(0)

#define minxvpudbgprint(f)                                                  \
    do {                                                                    \
        ConfigurationValue *cv = minx_config_get(CONF_MVPU_DEBUGGING);      \
        if(cv != NULL && cv->b) {                                           \
            printf(MINX_VPU_PRINT_PREFIX": "f);                             \
        }                                                                   \
    }while(0)

#define EXPLAIN_FUNCTION()                                                  \
    do{                                                                     \
        ConfigurationValue *cv = minx_config_get(CONF_SRC_DEBUGGING);       \
        if(cv != NULL && cv->b) {                                           \
            printf(MINX_VPU_FUNC_PRINT_PREFIX": %s, line %s\n", __func__,   \
                    __LINE__);                                              \
        }                                                                   \
    } while(0)

#define EXPLAIN_OPCODE(opc)                                                 \
    do {                                                                    \
        ConfigurationValue *cv = minx_config_get(CONF_SRC_DEBUGGING);       \
        if(cv != NULL && cv->b) {                                           \
            printf(MINX_VPU_OP_PRINT_PREFIX": %s\n", opc);                  \
        }                                                                   \
    } while(0)

#define EXPLAIN_OPCODE_WITH(opc,format,...)                                 \
    do {                                                                    \
        ConfigurationValue *cv = minx_config_get(CONF_SRC_DEBUGGING);       \
        if(cv != NULL && cv->b) {                                           \
            printf(MINX_VPU_OP_PRINT_PREFIX": %s ("format")\n", opc,        \
                    __VA_ARGS__);                                           \
        }                                                                   \
    } while(0)

#define EXPLAIN_OPCODE_WITH_HEXF_WITH(opc,hexf,nonhexf,...)                 \
    do {                                                                    \
        if(minx_config_get(CONF_SRC_DEBUGGING)->b) {                        \
            if(minx_config_get(CONF_HEX)->b)                                \
                printf(MINX_VPU_OP_PRINT_PREFIX": %s ("hexf")\n",           \
                        opc, __VA_ARGS__);                                  \
            else                                                            \
                printf(MINX_VPU_OP_PRINT_PREFIX": %s ("nonhexf")\n",        \
                        opc, __VA_ARGS__);                                  \
        }                                                                   \
    } while(0)

#endif //__MINX_DEBUG_H__

#endif //DEBUG
