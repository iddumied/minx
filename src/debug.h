#if (defined DEBUG || defined DEBUGGING)

#ifndef __MINX_DEBUG_H__
#define __MINX_DEBUG_H__

#include <stdio.h>

#define minxvmdbgprint(f,...)                                               \
    do {                                                                    \
        if(minx_config_is_set(CONF_MVM_DEBUGGING)) {                        \
            printf("[minx][vm] "f,__VA_ARGS__);                             \
        }                                                                   \
    }while(0)

#define EXPLAIN_FUNCTION()                                                  \
    do{                                                                     \
        printf("[minx][vm] func: %s, line %s\n", __func__, __LINE__);       \
    } while(0)

#define EXPLAIN_OPCODE(opc)                                                 \
    do {                                                                    \
        printf("[minx][vm] opcode: %s\n", opc);                             \
    } while(0)

#define EXPLAIN_OPCODE_WITH(opc,format,...)                                 \
    do {                                                                    \
        printf("[minx][vm] opcode: %s ("format")\n", opc, __VA_ARGS__);     \
    } while(0)

#endif //__MINX_DEBUG_H__

#endif //DEBUG
