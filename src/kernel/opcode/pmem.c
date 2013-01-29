#include "opcodes.h"

/*
 * Command:                 PMEM 
 * Parameters:              1, heapnode-address
 * Affects Program Pointer: NO
 *
 *
 */
void opc_pmem_func(void) {
#ifdef DEBUGGING
    if( minx_config_get(CONF_SRC_DEBUGGING)->b ) {
        unsigned int params[] = { REGISTER_ADDRESS_SIZE };
        read_n_command_parameters(1, params);

        EXPLAIN_OPCODE_WITH("pmem", "memory: %"PRIu64, minx_registers_find_register(opc_p->p[0])->value);

        minx_kernel_heap_print_heapnode(minx_registers_find_register(opc_p->p[0])->value);
    }
#endif
}

