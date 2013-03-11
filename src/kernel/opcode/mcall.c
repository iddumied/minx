#include "kernel/opcodes.h"

/*
 * Command:                 MCALL
 * Parameters:              2: moduleID, opcode, heap-address
 * Affects Program Pointer: NO
 *
 */
void minx_opc_mcall_func(uint64_t *params) {
    Register *mod_id_reg = minx_registers_find_register(params[0]);
    Register *mod_op_reg = minx_registers_find_register(params[1]);
    HeapNode *heap = minx_kernel_heap_get(params[2]);

#ifdef DEBUGGING
    EXPLAIN_OPCODE("Call module %i",mod_id_reg->value);
#endif

    minx_kernel_module_call_opcode(mod_id_reg->value, mod_op_reg->value, heap);
}
