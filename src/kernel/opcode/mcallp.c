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
    EXPLAIN_OPCODE_WITH_HEXF(
            "Call module %#010"PRIx64" without parameters",
            "Call module %"PRIu64" without parameters",
            mod_id_reg->value);
#endif

    minx_kernel_module_call_opcode_noparam(mod_id_reg->value, mod_op_reg->value);
}
