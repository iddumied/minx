#if (defined DEBUG || defined DEBUGGING)

#ifndef __MINX_DEBUG_H__
#define __MINX_DEBUG_H__

#include <stdio.h>

#define minxvmdbgprint(f,...)                                               \
    do {                                                                    \
        ConfigurationValue *cv = minx_config_get(CONF_MVM_DEBUGGING);       \
        if(cv != NULL && cv->b) {                                           \
            printf("[minx][vm] "f,__VA_ARGS__);                             \
        }                                                                   \
    }while(0)

#define EXPLAIN_FUNCTION()                                                  \
    do{                                                                     \
        ConfigurationValue *cv = minx_config_get(CONF_SRC_DEBUGGING);       \
        if(cv != NULL && cv->b) {                                           \
            printf("[minx][vm] func: %s, line %s\n", __func__, __LINE__);   \
        }                                                                   \
    } while(0)

#define EXPLAIN_OPCODE(opc)                                                 \
    do {                                                                    \
        ConfigurationValue *cv = minx_config_get(CONF_SRC_DEBUGGING);       \
        if(cv != NULL && cv->b) {                                           \
            printf("[minx][vm] opcode: %s\n", opc);                         \
        }                                                                   \
    } while(0)

#define EXPLAIN_OPCODE_WITH(opc,format,...)                                 \
    do {                                                                    \
        ConfigurationValue *cv = minx_config_get(CONF_SRC_DEBUGGING);       \
        if(cv != NULL && cv->b) {                                           \
            printf("[minx][vm] opcode: %s ("format")\n", opc, __VA_ARGS__); \
        }                                                                   \
    } while(0)

#endif //__MINX_DEBUG_H__

#endif //DEBUG
