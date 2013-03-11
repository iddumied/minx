#include "kernel/opcodes.h"

/**
 * @brief Opcode IMPORT
 *
 * Command:                 IMPORT
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_import_func(uint64_t *params) {
#ifdef DEBUGGING
#endif //DEBUGGING

    minx_registers_pop();
}

