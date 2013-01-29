#include "kernel/opcodes.h"

/*
 * Command:                 EQL 
 * Parameters:              1: register-address register-address
 * Affects Program Pointer: NO
 *
 *
 */
void opc_eql_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("eql", "reg %"PRIu64" and reg %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif 
    
    minx_registers_find_register(akku_register_number)->value = minx_registers_find_register(opc_p->p[0])->value == minx_registers_find_register(opc_p->p[0])->value;

}
