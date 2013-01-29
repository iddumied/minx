#include "opcodes.h"

/*
 * Command:                 DEC
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
void opc_dec_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("dec", "reg: %"PRIu64, opc_p->p[0]);
#endif 

    if( minx_registers_find_register(opc_p->p[0])->value == 0x0000 ) {
        setbit(statusregister, OVERFLOW_BIT);
    }

    minx_registers_find_register(opc_p->p[0])->value--;
    
}
