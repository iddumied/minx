#include "kernel/opcodes.h"

/*
 * Command:                 EXITI
 * Parameters:              1: exit with value
 * Affects Program Pointer: NO
 *
 */
void minx_opc_exiti_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("code: %"PRIu64, params[0]);
#endif

    minx_kernel_unset_running_variable();
    minx_kernel_set_exit_status(params[0]);
}
