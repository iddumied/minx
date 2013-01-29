#include "kernel/opcodes.h"

/*
 * Command:                 CALL
 * Parameters:              1, program address
 * Affects Program Pointer: YES
 *
 * get current program_pointer, add enough for the next opcode, push it to stack,
 * set the program_pointer to the value of the argument of the opc.
 */
void opc_call_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("%"PRIu64, opc_p->p[1]);
#endif
    /*
     * push to stack
     *
     * push the address of the next opcode to the stack,
     * the address has size (PROGRAM_ADDRESS_SIZE) (in this case this is 8 Byte)
     */
    stackpush(  stack,
                (void*)(minx_kernel_program_pointer_get() + OPC_SIZE + PROGRAM_ADDRESS_SIZE), 
                (size_t)PROGRAM_ADDRESS_SIZE);

    minx_kernel_program_pointer_manipulate(opc_p->p[0]);
}

