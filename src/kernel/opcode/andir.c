#include "opcodes.h"

/*
 * Command:                 ANDIR
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 * Result in first register
 */
void opc_andir_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("andir", "reg %"PRIu64" & %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif

    minx_registers_find_register(opc_p->p[0])->value &= opc_p->p[1];
}
