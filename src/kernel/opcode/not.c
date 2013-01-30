#include "kernel/opcodes.h"

/*
 * Command:                 NOT 
 * Parameters:              1: register-address 
 * Affects Program Pointer: NO
 *
 * Result in minx_registers_find_register(akku_register_number)->value
 */
void opc_not_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("reg: %"PRIu64, params[1]);
#endif 

    minx_registers_find_register(akku_register_number)->value = 
        ! minx_registers_find_register(params[1])->value;
}
