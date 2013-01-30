#include "kernel/opcodes.h"

/*
 * Command:                 RSH
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
void opc_rsh_func(uint64_t *params) {

#ifdef DEBUGGING
    EXPLAIN_OPCODE("reg %"PRIu64, params[0]);
#endif

    Register *r = minx_registers_find_register(params[0]);
    r->value = r->value>>1;
    
}
