#include "kernel/opcodes.h"

/*
 * Command:                 NOT 
 * Parameters:              1: register-address 
 * Affects Program Pointer: NO
 *
 * Result in minx_registers_find_register(akku_register_number)->value
 */
void opc_not_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("not", "reg: %"PRIu64, opc_p->p[1]);
#endif 

    minx_registers_find_register(akku_register_number)->value = 
        ! minx_registers_find_register(opc_p->p[1])->value;
}
