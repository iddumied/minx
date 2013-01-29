#include "kernel/opcodes.h"

/*
 * Command:                 JMPNZ
 * Parameters:              2: register-address, address
 * Affects Program Pointer: YES
 *
 *
 */
void opc_jmpnz_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF_WITH("jmpnz",
            "to %#010"PRIx64" if reg %"PRIu64" != 0",
            "to %"PRIu64" if reg %"PRIu64" != 0", 
            opc_p->p[1], 
            opc_p->p[0]);
#endif 

    if( minx_binary_exists_at(opc_p->p[1]) || opc_p->p[1] == END_OF_PROGRAM) {
        if( minx_registers_find_register(opc_p->p[0])->value != 0x0000 ) {
            minx_kernel_program_pointer_manipulate(opc_p->p[1]);
        }
        else {
            minx_kernel_program_pointer_manipulate(OPC_SIZE + REGISTER_ADDRESS_SIZE + PROGRAM_ADDRESS_SIZE);
        }
    } 
    else {
        FATAL_DESC_ERROR("Cannot jump");
    }
}
