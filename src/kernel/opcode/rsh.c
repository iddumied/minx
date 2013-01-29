#include "kernel/opcodes.h"

/*
 * Command:                 RSH
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
void opc_rsh_func() {

#ifdef DEBUGGING
    EXPLAIN_OPCODE("reg %"PRIu64, opc_p->p[0]);
#endif

    Register *r = minx_registers_find_register(opc_p->p[0]);
    r->value = r->value>>1;
    
}
