#include "kernel/opcodes.h"

/*
 * Command:                 PSTACK
 * Parameters:              0
 * Affects Program Pointer: NO 
 */
void opc_pstack_func (void) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("pstack");
    if( minx_config_get(CONF_SRC_DEBUGGING)->b ) {
        stack_print_binary(stack);
    }

#endif //DEBUGGING
}
