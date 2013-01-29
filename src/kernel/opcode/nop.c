#include "opcodes.h"

/*
 * Command:                 NOP
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 *
 */
void opc_nop_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("nop");
#endif
    program_pointer += (OPC_SIZE);
}
