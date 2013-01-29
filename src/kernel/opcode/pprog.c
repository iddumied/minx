#include "kernel/opcodes.h"

/*
 * Command:                 PPROG
 * Parameters:              0
 * Affects Program Pointer: NO 
 */
void opc_pprog_func (void) {
#ifdef DEBUGGING
    /* nothing to explain here */

    if( minx_config_get(CONF_SRC_DEBUGGING)->b ) {
        minx_binary_print();
    }

#endif //DEBUGGING
}
