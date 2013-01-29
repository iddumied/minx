#include "kernel/opcodes.h"

/*
 * Command:                 OR
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 * Result in minx_registers_find_register(akku_register_number)->value
 */
void opc_or_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("or", "reg %"PRIu64" | reg %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif 

    minx_registers_find_register(akku_register_number)->value = 
        minx_registers_find_register(opc_p->p[0])->value 
        
        | 
        
        minx_registers_find_register(opc_p->p[1])->value;
}
