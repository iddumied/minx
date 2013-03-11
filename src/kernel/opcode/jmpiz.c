#include "kernel/opcodes.h"

/**
 * @brief Opcode JMPIZ
 *
 * Command:                 JMPIZ
 * Parameters:              2: register-address, address
 * Affects Program Pointer: YES
 *
 *
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_jmpiz_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
            "to %#010"PRIx64" if R%"PRIu64" == 0",
            "to %"PRIu64" if R%"PRIu64" == 0", 
            params[1], 
            params[0]);
#endif 

    if (minx_binary_exists_at(params[1]) || params[1] == END_OF_PROGRAM) {
        if (minx_registers_find_register(params[0])->value == 0x0000 ) {
            minx_kernel_program_pointer_manipulate(params[1]);
        }
    }
    else {
        FATAL_DESC_ERROR("Cannot jump");
    }
}

