#include "kernel/opcodes.h"

/**
 * @brief Opcode MGETPC
 *
 * Command:                 MGETPC
 * Parameters:              2: moduleID, opcode
 * Affects Program Pointer: NO
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_mgetpc_func(uint64_t *params) {
    Register *mod_id_reg = minx_registers_find_register(params[0]);
    Register *mod_op_reg = minx_registers_find_register(params[1]);

    Register *akku = minx_registers_find_register(akku_register_number);

    akku->value = minx_kernel_module_get_parameter_count(mod_id_reg->value, mod_op_reg->value): 
}
