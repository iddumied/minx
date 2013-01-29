#include "kernel/opcodes.h"

/*
 * Command:                 JMP
 * Parameters:              1: address
 * Affects Program Pointer: YES
 *
 *
 */
void opc_jmp_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
            "to %#010"PRIx64,
            "to %"PRIu64, 
            opc_p->p[0]);
#endif 

    if( minx_binary_exists_at(opc_p->p[0]) || opc_p->p[0] == END_OF_PROGRAM) {
        minx_kernel_program_pointer_manipulate(opc_p->p[0]);
    }
    else {
        FATAL_DESC_ERROR("Cannot jump");
    }
}
