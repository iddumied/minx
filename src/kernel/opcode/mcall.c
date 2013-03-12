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

    Register *r1 = minx_registers_find_register(params[2]);
    HeapNode *heap = minx_kernel_heap_get(r1->value);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
            "Call module %#010"PRIx64,
            "Call module %"PRIu64,
            mod_id_reg->value);
#endif

    minx_kernel_module_call_opcode(mod_id_reg->value, mod_op_reg->value, heap);
}
