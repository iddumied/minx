#include "kernel/opcodes.h"

/*
 * Command:                 ANDR
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 * Result in first register
 */
void opc_andr_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("andr", "reg %"PRIu64" & reg%"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif

    minx_registers_find_register(opc_p->p[0])->value &= minx_registers_find_register(opc_p->p[1])->value;
}
