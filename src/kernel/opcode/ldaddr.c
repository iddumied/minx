#include "kernel/opcodes.h"

/**
 * @brief Opcode LDADDR
 *
 * Command:                 LDADDR
 * Parameters:              0:
 * Affects Program Pointer: NO
 *
 * @param params Pointer to parameters for this opcode
 */

void minx_opc_ldaddr_func(uint64_t *params) {
    Register *prop = minx_registers_find_register(program_pointer_register_number);
    Register *akku = minx_registers_find_register(akku_register_number);

    akku->value = prop->value + OPC_SIZE;

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
            "load address of next opcode (%#010"PRIx64") into akku", 
            "load address of next opcode (%"PRIu64") into akku", 
            akku->value);
#endif

}

