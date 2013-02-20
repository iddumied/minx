#include "kernel/opcodes.h"

/*
 * Command:                 MCALL
 * Parameters:              2: moduleID, opcode, heap-address
 * Affects Program Pointer: NO
 *
 */
void minx_opc_mcall_func(uint64_t *params) {
    HeapNode *heap = minx_kernel_heap_get(params[2]);
    minx_kernel_module_call_opcode(params[0], params[1], heap);
}
