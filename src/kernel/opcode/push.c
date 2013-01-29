#include "opcodes.h"

/*
 * Command:                 PUSH
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
void opc_push_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("push", "reg %"PRIu64, opc_p->p[0]);
#endif

    stackpush(stack, &(minx_registers_find_register(opc_p->p[0])->value), VALUE_SIZE) ;
}
