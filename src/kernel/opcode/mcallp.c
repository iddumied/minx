#include "kernel/opcodes.h"

/*
 * Command:                 MCALLP
 * Parameters:              2: moduleID, opcode
 * Affects Program Pointer: NO
 *
 */
void minx_opc_mcallnp_func(uint64_t *params) {
    Register *mod_id_reg = minx_registers_find_register(params[0]);
    Register *mod_op_reg = minx_registers_find_register(params[1]);

#ifdef DEBUGGING
    EXPLAIN_OPCODE("Call module %i without parameters",mod_id_reg->value);
#endif

    minx_kernel_module_call_opcode_noparam(mod_id_reg->value, mod_op_reg->value);
}
