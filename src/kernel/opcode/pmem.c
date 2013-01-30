#include "kernel/opcodes.h"

/*
 * Command:                 PMEM 
 * Parameters:              1, heapnode-address
 * Affects Program Pointer: NO
 *
 *
 */
void opc_pmem_func(uint64_t* params) {
#ifdef DEBUGGING
    if( minx_config_get(CONF_SRC_DEBUGGING)->b ) {
        EXPLAIN_OPCODE("memory: %"PRIu64, minx_registers_find_register(params[0])->value);

        minx_kernel_heap_print_heapnode(minx_registers_find_register(params[0])->value);
    }
#endif
}

