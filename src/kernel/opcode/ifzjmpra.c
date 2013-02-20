#include "kernel/opcodes.h"

/*
 * Command:                 IFZJMPRA
 * Parameters:              2: register-addres, address
 * Affects Program Pointer: NO
 *
 */
void minx_opc_ifzjmpra_func(uint64_t *params) {
    
    Register *r1 = minx_registers_find_register(params[0]);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
            "to %#010"PRIx64" if akku == 0, else to %#010"PRIx64,
            "to %"PRIu64" if akku == 0, else to %"PRIu64, 
            r1->value, 
            params[1]);
#endif

    
    if( (minx_binary_exists_at(r1->value) || r1->value == END_OF_PROGRAM) && 
        (minx_binary_exists_at(params[1]) || params[1] == END_OF_PROGRAM)) {

        if( minx_registers_find_register(akku_register_number)->value == 0x0000 ) {
            minx_kernel_program_pointer_manipulate(r1->value);
        }
        else {
            minx_kernel_program_pointer_manipulate(params[1]);
        }
    } 
    else {
        FATAL_DESC_ERROR("Cannot jump");
    }

}

