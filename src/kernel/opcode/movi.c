#include "opcodes.h"

/*
 * Command:                 MOVI
 * Parameters:              2: register-address value
 * Affects Program Pointer: NO
 *
 *
 */
void opc_movi_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF_WITH("movi",
            "R%"PRIu64" <- %#010"PRIx64, 
            "R%"PRIu64" <- %"PRIu64, 
            opc_p->p[0], opc_p->p[1]);
#endif 

    minx_registers_find_register(opc_p->p[0])->value = opc_p->p[1];
}
