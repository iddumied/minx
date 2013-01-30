#include "kernel/opcodes.h"

/*
 * Command:                 EXIT
 * Parameters:              1: register-address
 * Affects Program Pointer: NO 
 */
void opc_exit_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("code: %"PRIu64, minx_registers_find_register(params[0])->value);
#endif

    minx_kernel_unset_running_variable();
    minx_kernel_set_exit_status(minx_registers_find_register(params[0])->value);
}
