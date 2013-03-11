#include "kernel/opcodes.h"

/*
 * Command:                 MUNLOAD
 * Parameters:              1: moduleID
 * Affects Program Pointer: NO
 *
 */
void minx_opc_munload_func(uint64_t *params) {
    Register *mod_id_reg = minx_registers_find_register(params[0]);

#ifdef DEBUGGING
    EXPLAIN_OPCODE("Unload module %i", mod_id_reg->value);
#endif

    minx_kernel_module_unload(mod_id_reg->value);
}
