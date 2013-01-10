#ifdef DEBUG

#ifndef __MINX_DEBUG_H__
#define __MINX_DEBUG_H__

#define EXPLAIN_FUNCTION()                                                  \
    do{                                                                     \
        printf("[minx][vm] func: %s, line %s\n", __func__, __LINE__);       \
    } while(0)

#define EXPLAIN_OPCODE(opc)                                                 \
    do {                                                                    \
        printf("[minx][vm] opcode: %s\n", opc);                             \
    } while(0)

#define EXPLAIN_OPCODE_WITH(opc,format,val)                                 \
    do {                                                                    \
        printf("[minx][vm] opcode: %s ("format")\n", opc, val );            \
    } while(0)

#endif //__MINX_DEBUG_H__

#endif //DEBUG
