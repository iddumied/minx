#include "opcodes.h"

/*
 * Command:                 ADDI
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 * Result in akku
 */
void opc_addi_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("addi", "reg %"PRIu64" + %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif

    minx_registers_find_register(akku_register_number) = 
        minx_registers_find_register(opc_p->p[0])->value + opc_p->p[1];

}
