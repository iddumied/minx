#include "opcodes.h"

/*
 * Command:                 POP
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
void opc_pop_func() {
    if( stack_is_empty( stack ) ) {
        FATAL_DESC_ERROR("Cannot pop from empty stack!");
    }

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("pop", "reg %"PRIu64, opc_p->p[0]);
#endif

    minx_registers_find_register(opc_p->p[0])->value = *((uint64_t*)stackpop(stack));
}
