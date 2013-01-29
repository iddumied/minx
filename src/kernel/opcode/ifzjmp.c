#include "kernel/opcodes.h"

/*
 * Command:                 IFZJMP
 * Parameters:              2: address, address 
 * Affects Program Pointer: YES
 *
 * if akku is zero then jump else jump
 */
void opc_ifzjmp_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
            "to %#010"PRIx64" if akku == 0, else to %#010"PRIx64,
            "to %"PRIu64" if akku == 0, else to %"PRIu64, 
            opc_p->p[0], 
            opc_p->p[1]);
#endif 

    if( (minx_binary_exists_at(opc_p->p[0]) || opc_p->p[0] == END_OF_PROGRAM) && 
        (minx_binary_exists_at(opc_p->p[1]) || opc_p->p[1] == END_OF_PROGRAM)) {

        if( minx_registers_find_register(akku_register_number)->value == 0x0000 ) {
            minx_kernel_program_pointer_manipulate(opc_p->p[0]);
        }
        else {
            minx_kernel_program_pointer_manipulate(opc_p->p[1]);
        }
    } 
    else {
        FATAL_DESC_ERROR("Cannot jump");
    }
}
