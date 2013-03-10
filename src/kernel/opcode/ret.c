#include "kernel/opcodes.h"

/**
 * @brief Opcode RET
 *
 * Command:                 RET
 * Parameters:              0
 * Affects Program Pointer: YES
 *
 *
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_ret_func(uint64_t *params) {
#ifdef DEBUGGING
    /* nothing to explain here */
#endif
    if (stack_is_empty(stack))
        FATAL_DESC_ERROR("Cannot RET, stack is empty!");
     
    minx_kernel_program_pointer_manipulate( *((uint64_t*) stackpop(stack)));
}

