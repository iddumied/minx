#include "kernel/opcodes.h"

/***
 * @brief Opcode EXITI
 *
 * Command:                 EXITI
 * Parameters:              1: exit with value
 * Affects Program Pointer: NO
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_exiti_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
            "code: %#010"PRIx64, 
            "code: %"PRIu64, 
            params[0]);
#endif

    minx_kernel_unset_running_variable();
    minx_kernel_set_exit_status(params[0]);
}
