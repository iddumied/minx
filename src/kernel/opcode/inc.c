#include "opcodes.h"

/*
 * Command:                 INC
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
void opc_inc_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("inc", "reg: %"PRIu64, opc_p->p[0]);
#endif 

    if( minx_registers_find_register(opc_p->p[0])->value == 0xFFFF ) {
        setbit(statusregister, OVERFLOW_BIT);
    }

    minx_registers_find_register(opc_p->p[0])->value++;
}
