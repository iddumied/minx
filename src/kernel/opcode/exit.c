#include "kernel/opcodes.h"

/*
 * Command:                 EXIT
 * Parameters:              1: register-address
 * Affects Program Pointer: NO 
 */
void opc_exit_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("exit", "code: %i", minx_registers_find_register(opc_p->p[0])->value);
#endif

    __running__     = 0;
    __exit_code__   = minx_registers_find_register(opc_p->p[0])->value;
}
