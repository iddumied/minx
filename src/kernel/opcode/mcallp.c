#include "kernel/opcodes.h"

/*
 * Command:                 MCALLP
 * Parameters:              2: moduleID, opcode
 * Affects Program Pointer: NO
 *
 */
void minx_opc_mcallp_func(uint64_t *params) {
    minx_kernel_module_call_opcode_noparam(params[0], params[1]);
}
