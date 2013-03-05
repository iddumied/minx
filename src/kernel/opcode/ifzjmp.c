#include "kernel/opcodes.h"

/**
 * @brief Opcode IFZJMP
 *
 * Command:                 IFZJMP
 * Parameters:              2: address, address 
 * Affects Program Pointer: YES
 *
 * if akku is zero then jump else jump
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_ifzjmp_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
            "to %#010"PRIx64" if akku == 0, else to %#010"PRIx64,
            "to %"PRIu64" if akku == 0, else to %"PRIu64, 
            params[0], 
            params[1]);
#endif 

    if( (minx_binary_exists_at(params[0]) || params[0] == END_OF_PROGRAM) && 
        (minx_binary_exists_at(params[1]) || params[1] == END_OF_PROGRAM)) {

        if( minx_registers_find_register(akku_register_number)->value == 0x0000 ) {
            minx_kernel_program_pointer_manipulate(params[0]);
        }
        else {
            minx_kernel_program_pointer_manipulate(params[1]);
        }
    } 
    else {
        FATAL_DESC_ERROR("Cannot jump");
    }
}
