#include "opcodes.h"

/*
 * Command:                 ADD
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 * Result in akku
 */
void opc_add_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("add", "reg %"PRIu64" + reg %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif

    minx_registers_find_register(akku_register_number)_= 
        minx_registers_find_register(opc_p->p[0])->value 
        + 
        minx_registers_find_register(opc_p->p[1])->value;
}
