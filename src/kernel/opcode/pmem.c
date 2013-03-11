#include "kernel/opcodes.h"

/**
 * @brief Opcode PMEM 
 *
 * Command:                 PMEM 
 * Parameters:              1, heapnode-address
 * Affects Program Pointer: NO
 *
 *
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_pmem_func(uint64_t* params) {
#ifdef DEBUGGING
    if( minx_config_get(CONF_SRC_DEBUGGING)->b ) {
        EXPLAIN_OPCODE_WITH_HEXF(
                "memory: %#010"PRIx64, 
                "memory: %"PRIu64, 
                minx_registers_find_register(params[0])->value);

        minx_kernel_heap_print_heapnode(minx_registers_find_register(params[0])->value);
    }
#endif
}

