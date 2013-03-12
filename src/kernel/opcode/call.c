#include "kernel/opcodes.h"

/**
 * @brief Opcode CALL
 *
 * Command:                 CALL
 * Parameters:              1, program address
 * Affects Program Pointer: YES
 *
 * get current program_pointer, add enough for the next opcode, push it to stack,
 *
 * @param params Pointer to parameters for this opcode
 * set the program_pointer to the value of the argument of the opc.
 */
void minx_opc_call_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF("%#010"PRIx64, "%"PRIu64, params[1]);
#endif
    /*
     * push to stack
     *
     * push the address of the next opcode to the stack,
     * the address has size (PROGRAM_ADDRESS_SIZE) (in this case this is 8 Byte)
     */

    uint64_t *data = (uint64_t*) malloc(sizeof(uint64_t));
    *data = minx_kernel_program_pointer_get() + OPC_SIZE + PROGRAM_ADDRESS_SIZE;

    stackpush(  stack,
                (void*)data, 
                (size_t)PROGRAM_ADDRESS_SIZE);

    minx_kernel_program_pointer_manipulate(params[0]);
}

