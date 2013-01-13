#if (defined DEBUG || defined DEBUGGING)

#ifndef __MINX_DEBUG_H__
#define __MINX_DEBUG_H__

#include <stdio.h>

#include "minx.h"

#define minxvmdbgprintf(f,...)                                              \
    do {                                                                    \
        ConfigurationValue *cv = minx_config_get(CONF_MVM_DEBUGGING);       \
        if(cv != NULL && cv->b) {                                           \
            printf(MINX_VM_PRINT_PREFIX" "f,__VA_ARGS__);                   \
        }                                                                   \
    }while(0)

#define minxvmdbgprint(f)                                                   \
    do {                                                                    \
        ConfigurationValue *cv = minx_config_get(CONF_MVM_DEBUGGING);       \
        if(cv != NULL && cv->b) {                                           \
            printf(MINX_VM_PRINT_PREFIX" "f);                               \
        }                                                                   \
    }while(0)

#define EXPLAIN_FUNCTION()                                                  \
    do{                                                                     \
        ConfigurationValue *cv = minx_config_get(CONF_SRC_DEBUGGING);       \
        if(cv != NULL && cv->b) {                                           \
            printf(MINX_VM_FUNC_PRINT_PREFIX": %s, line %s\n", __func__,    \
                    __LINE__);                                              \
        }                                                                   \
    } while(0)

#define EXPLAIN_OPCODE(opc)                                                 \
    do {                                                                    \
        ConfigurationValue *cv = minx_config_get(CONF_SRC_DEBUGGING);       \
        if(cv != NULL && cv->b) {                                           \
            printf(MINX_VM_OP_PRINT_PREFIX": %s\n", opc);                   \
        }                                                                   \
    } while(0)

#define EXPLAIN_OPCODE_WITH(opc,format,...)                                 \
    do {                                                                    \
        ConfigurationValue *cv = minx_config_get(CONF_SRC_DEBUGGING);       \
        if(cv != NULL && cv->b) {                                           \
            printf(MINX_VM_OP_PRINT_PREFIX": %s ("format")\n", opc,         \
                    __VA_ARGS__);                                           \
        }                                                                   \
    } while(0)

#endif //__MINX_DEBUG_H__

#endif //DEBUG
