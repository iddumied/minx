#include "kernel/opcodes.h"

/*
 * Command:                 NOTR
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 * Result in same register
 */
void opc_notr_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("not", "reg: %"PRIu64, opc_p->p[0]);
#endif 

    minx_registers_find_register(opc_p->p[0])->value = ! minx_registers_find_register(opc_p->p[0])->value;
}
