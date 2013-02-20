#include "kernel/opcodes.h"

/*
 * Command:                 MHASPRMS
 * Parameters:              2: moduleID, opcode
 * Affects Program Pointer: NO
 *
 */
void minx_opc_mhasprms_func(uint64_t *params) {
    Register *akku = minx_register_find_register(akku_register_number);
    akku->value = minx_module_opcode_gets_params(params[0], params[1]);
}
