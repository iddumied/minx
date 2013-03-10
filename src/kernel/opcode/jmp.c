#include "kernel/opcodes.h"

/**
 * @brief Opcode JMP
 *
 * Command:                 JMP
 * Parameters:              1: address
 * Affects Program Pointer: YES
 *
 *
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_jmp_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
            "to %#010"PRIx64,
            "to %"PRIu64, 
            params[0]);
#endif 

    if( minx_binary_exists_at(params[0]) || params[0] == END_OF_PROGRAM) {
        minx_kernel_program_pointer_manipulate(params[0]);
    }
    else {
        FATAL_DESC_ERROR("Cannot jump");
    }
}
