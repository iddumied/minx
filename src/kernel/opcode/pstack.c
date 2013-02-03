#include "kernel/opcodes.h"

/*
 * Command:                 PSTACK
 * Parameters:              0
 * Affects Program Pointer: NO 
 */
void minx_opc_pstack_func(uint64_t *params) {
#ifdef DEBUGGING
    /*nothing to explain here */

    if( minx_config_get(CONF_SRC_DEBUGGING)->b ) {
        stack_print_binary(stack);
    }

#endif //DEBUGGING
}
