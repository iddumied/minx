#include "kernel/opcodes.h"

/*
 * Command:                 EQL 
 * Parameters:              1: register-address register-address
 * Affects Program Pointer: NO
 *
 *
 */
void opc_eql_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("reg %"PRIu64" and reg %"PRIu64, params[0], params[1]);
#endif 
    
    minx_registers_find_register(akku_register_number)->value = 
        minx_registers_find_register(params[0])->value 
        == 
        minx_registers_find_register(params[1])->value;

}
