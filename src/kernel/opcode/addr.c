#include "kernel/opcodes.h"

/*
 * Command:                 ADDR
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 * Result in register
 */
void opc_addr_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("addr", "reg %"PRIu64" + reg %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif

    minx_registers_find_register(opc_p->p[0])->value += minx_registers_find_register(opc_p->p[1])->value;

}
