#include "kernel/opcodes.h"

/*
 * Command:                 MOV 
 * Parameters:              2: register-adress register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_mov_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
            "R%"PRIu64" <_ R%"PRIu64" (%#010"PRIx64")",
            "R%"PRIu64" <_ R%"PRIu64" (%"PRIu64")",
                opc_p->p[0], 
                opc_p->p[1],
                minx_registers_find_register(opc_p->p[1])->value); 
#endif 

    minx_registers_find_register(opc_p->p[0])->value = minx_registers_find_register(opc_p->p[1])->value;
}

