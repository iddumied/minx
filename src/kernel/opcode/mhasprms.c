#include "kernel/opcodes.h"

/*
 * Command:                 MHASPRMS
 * Parameters:              2: moduleID, opcode
 * Affects Program Pointer: NO
 *
 */
void minx_opc_mhasprms_func(uint64_t *params) {
    Register *akku = minx_registers_find_register(akku_register_number);
    Register *mod_id_reg = minx_registers_find_register(params[0]);
    Register *mod_op_reg = minx_registers_find_register(params[1]);

    akku->value = minx_kernel_module_opcode_gets_params(mod_id_reg->value, mod_op_reg->value);
}
