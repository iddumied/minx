#include "kernel/opcodes.h"

/**
 * @brief Opcode IFZJMPRR
 *
 * Command:                 IFZJMPRR
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_ifzjmprr_func(uint64_t *params) {
    
    Register *r1 = minx_registers_find_register(params[0]);
    Register *r2 = minx_registers_find_register(params[1]);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
            "to %#010"PRIx64" if akku == 0, else to %#010"PRIx64,
            "to %"PRIu64" if akku == 0, else to %"PRIu64, 
            r1->value, 
            r2->value);
#endif

    
    if( (minx_binary_exists_at(r1->value) || r1->value == END_OF_PROGRAM) && 
        (minx_binary_exists_at(r2->value) || r2->value == END_OF_PROGRAM)) {

        if( minx_registers_find_register(akku_register_number)->value == 0x0000 ) {
            minx_kernel_program_pointer_manipulate(r1->value);
        }
        else {
            minx_kernel_program_pointer_manipulate(r2->value);
        }
    } 
    else {
        FATAL_DESC_ERROR("Cannot jump");
    }

}

