#include "opcodes.h"

/*
 * Command:                 ORIR
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 *
 */
void opc_orir_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("orir", "reg %"PRIu64" | %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif 

    minx_registers_find_register(opc_p->p[0])->value |= opc_p->p[1];
}
