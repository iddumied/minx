#include "opcodes.h"

/*
 * Command:                 CMPI 
 * Parameters:              1: register-address value 
 * Affects Program Pointer: NO
 *
 * compare, 
 * result in minx_registers_find_register(akku_register_number)->value, 
 * 1 if first is bigger, 
 * 2 if second is bigger, 
 * zero if equal
 */
void opc_cmpi_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("cmpi", "reg %"PRIu64" and %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif 
    
    Register * r1 = minx_registers_find_register(opc_p->p[0]);

    if(r1->value > opc_p->p[1]) {
        minx_registers_find_register(akku_register_number)->value = 1;
    }
    else if(r1->value < opc_p->p[1]) {
        minx_registers_find_register(akku_register_number)->value = 2;
    }
    else {//( r1->value == r2->value )
        minx_registers_find_register(akku_register_number)->value = 0;
    }

}
