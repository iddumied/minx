#include "kernel/opcodes.h"

/*
 * Command:                 ADDIR
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 * Result in register
 */
void opc_addir_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("addir", "reg %"PRIu64" + %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif

    minx_registers_find_register(opc_p->p[0])->value += opc_p->p[1];

}
