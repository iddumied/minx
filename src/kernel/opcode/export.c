#include "kernel/opcodes.h"

/**
 * @brief Opcode EXPORT
 *
 * Command:                 EXPORT
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_export_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("all registers");
#endif //DEBUGGING

    minx_registers_push();
}
