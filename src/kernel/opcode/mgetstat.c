#include "kernel/opcodes.h"

/*
 * Command:                 MGETSTAT
 * Parameters:              1: moduleID
 * Affects Program Pointer: NO
 *
 */
void minx_opc_mgetstat_func(uint64_t *params) {
    Register *akku = minx_register_find_register(akku_register_number);
    akku->value = minx_kernel_module_get_status(params[0]);
}
