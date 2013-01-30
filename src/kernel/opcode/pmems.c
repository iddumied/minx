#include "kernel/opcodes.h"

/*
 * Command:                 PMEMS
 * Parameters:              0
 * Affects Program Pointer: NO 
 */
void opc_pmems_func(uint64_t* params) {
#ifdef DEBUGGING
    /* nothing to explain here */

    if( minx_config_get(CONF_SRC_DEBUGGING)->b ) {
        minx_kernel_heap_print_heap();
    }
#endif
}
