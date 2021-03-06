#include "kernel/opcodes.h"

/**
 * @brief Opcode EXIT
 *
 * Command:                 EXIT
 * Parameters:              1: register-address
 * Affects Program Pointer: NO 
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_exit_func(uint64_t *params) {

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
            "code: %#010"PRIx64, 
            "code: %"PRIu64, 
            minx_registers_find_register(params[0])->value);
#endif

    minx_kernel_unset_running_variable();
    minx_kernel_set_exit_status(minx_registers_find_register(params[0])->value);
}
