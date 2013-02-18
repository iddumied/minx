#include "kernel/opcodes.h"

/*
 * Command:                 LDADDR
 * Parameters:              0:
 * Affects Program Pointer: NO
 *
 */
void minx_opc__func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("load address of next opcode into akku");
#endif

    Register *prop = minx_registers_find_register(program_pointer_register_number);
    Register *akku = minx_registers_find_register(akku_register_number);

    akku->value = prop->value + OPC_SIZE;
}

