#include "kernel/opcodes.h"

/*
 * Command:                 JMPR
 * Parameters:              2: register
 * Affects Program Pointer: NO
 *
 */
void minx_opc_jmpr_func(uint64_t *params) {

    Register *r1 = minx_registers_find_register(params[0]);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
            "to %#010"PRIx64,
            "to %"PRIu64, 
            r1->value);
#endif 

    if( minx_binary_exists_at(r1->value) || r1->value == END_OF_PROGRAM) {
        minx_kernel_program_pointer_manipulate(r1->value);
    }
    else {
        FATAL_DESC_ERROR("Cannot jump");
    }
}
