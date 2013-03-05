#include "kernel/opcodes.h"

/**
 * @brief Opcode JMPNZR
 *
 * Command:                 JMPNZR
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 * @param params Pointer to parameters for this opcode
 */

void minx_opc_jmpnzr_func(uint64_t *params) {

    Register *r1 = minx_registers_find_register(params[1]);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
            "to %#010"PRIx64" if reg %"PRIu64" == 0",
            "to %"PRIu64" if reg %"PRIu64" == 0", 
            r1->value, 
            params[0]);
#endif 

    if( minx_binary_exists_at(r1->value) || r1->value == END_OF_PROGRAM) {
        if( minx_registers_find_register(params[0])->value != 0x0000 ) {
            minx_kernel_program_pointer_manipulate(r1->value);
        }
    }
    else {
        FATAL_DESC_ERROR("Cannot jump");
    }
}

