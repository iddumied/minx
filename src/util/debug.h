#if (defined DEBUG || defined DEBUGGING)

#ifndef __MINX_DEBUG_H__
#define __MINX_DEBUG_H__

#include <stdio.h>

#include "util/config.h"
#include "util/print_prefixes.h"

/* helper macro */
#define minxdbgprintf(conf,pref,format,...)                                 \
    do {                                                                    \
        ConfigurationValue *cv = minx_config_get(conf);                     \
        if(cv != NULL && cv->b) {                                           \
            printf(pref": "format,__VA_ARGS__);                             \
        }                                                                   \
    }while(0)

/* debugging for binary */
#define minxbinarydbgprintf(f,...)                                          \
    minxdbgprintf(CONF_MINX_DEBUGGING,MINX_BINARY_PRINT_PREFIX,f"\n",__VA_ARGS__);

#define minxbinarydbgprint(f)                                               \
    do {                                                                    \
        ConfigurationValue *cv = minx_config_get(CONF_MINX_DEBUGGING);      \
        if(cv != NULL && cv->b) {                                           \
            printf(MINX_BINARY_PRINT_PREFIX": "f"\n");                      \
        }                                                                   \
    }while(0)

/* debugging for kernel */
#define minxkerneldbgprintf(f,...)                                          \
    minxdbgprintf(CONF_MINX_DEBUGGING,MINX_KERNEL_PRINT_PREFIX,f,__VA_ARGS__);

#define minxkerneldbgprint(f)                                               \
    do {                                                                    \
        ConfigurationValue *cv = minx_config_get(CONF_MINX_DEBUGGING);      \
        if(cv != NULL && cv->b) {                                           \
            printf(MINX_KERNEL_PRINT_PREFIX": "f);                          \
        }                                                                   \
    }while(0)

/* source debugging */
#define MINX_SRC_DEBUG(module,f,...)                                        \
    do {                                                                    \
        ConfigurationValue *cv = minx_config_get(CONF_SRC_DEBUGGING);       \
        if(cv != NULL && cv->b) {                                           \
            printf("%s: "f,module,__VA_ARGS__);                             \
        }                                                                   \
    } while(0)

#define EXPLAIN_FUNCTION()                                                  \
    do{                                                                     \
        ConfigurationValue *cv = minx_config_get(CONF_SRC_DEBUGGING);       \
        if(cv != NULL && cv->b) {                                           \
            printf(MINX_KERNEL_FUNC_PRINT_PREFIX": %s, line %s", __func__,  \
                    __LINE__);                                              \
        }                                                                   \
    } while(0)

/* opcodes */
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
