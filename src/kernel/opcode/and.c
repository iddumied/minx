#include "opcodes.h"

/*
 * Command:                 AND
 * Parameters:              2: register-address register-address
 * Affects Program Pointer: NO
 *
 * result in akku_register_number
 */
static void opc_and_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("and", "reg %"PRIu64" & reg %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif

    minx_registers_find_register(akku_register_number) = 
        minx_registers_find_register(opc_p->p[1])->value 
        & 
        minx_registers_find_register(opc_p->p[1])->value;

}

