#include "kernel/opcodes.h"

/*
 * Command:                 MGETSTAT
 * Parameters:              1: moduleID
 * Affects Program Pointer: NO
 *
 */
void minx_opc_mgetstat_func(uint64_t *params) {
    Register *akku = minx_registers_find_register(akku_register_number);
    Register *mod_id_reg = minx_registers_find_register(params[0]);

    akku->value = minx_kernel_module_get_status(mod_id_reg->value);
}
