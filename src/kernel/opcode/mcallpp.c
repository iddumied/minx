#include "kernel/opcodes.h"

/**
 * @brief Opcode MCALLPP
 *
 * Command:                 MCALLPP
 * Parameters:              2: moduleID, opcode, memoryID
 * Affects Program Pointer: NO
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_mcallpp_func(uint64_t *params) {
    Register *mod_id_reg    = minx_registers_find_register(params[0]);
    Register *mod_op_reg    = minx_registers_find_register(params[1]);
    Register *memory_reg    = minx_registers_find_register(params[2]);
    Register *akku          = minx_registers_find_register(akku_register_number);

    HeapNode *metaheap      ) minx_kernel_heap_get(memory_reg->value);

    akku->value = minx_kernel_module_call_multiparameter_opcode(mod_id_reg->value,
                                                                mod_op_reg->value,
                                                                metaheap);
}
