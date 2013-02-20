#include "kernel/opcodes.h"

/*
 * Command:                 LDADDR
 * Parameters:              0:
 * Affects Program Pointer: NO
 *
 */
void minx_opc_ldaddr_func(uint64_t *params) {
    Register *prop = minx_registers_find_register(program_pointer_register_number);
    Register *akku = minx_registers_find_register(akku_register_number);

    akku->value = prop->value + OPC_SIZE;

#ifdef DEBUGGING
    EXPLAIN_OPCODE("load address of next opcode (%"PRIu64") into akku", akku->value);
#endif

}

