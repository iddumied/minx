#include "opcodes.h"

/*
 * Command:                 CMP 
 * Parameters:              2: register-address register-address
 * Affects Program Pointer: NO
 *
 * compare, 
 * result in akku_register_number, 
 * 1 if first is bigger, 
 * 2 if second is bigger, 
 * zero if equal
 */
void opc_cmp_func() {

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("cmp", "reg %"PRIu64" and reg %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif 
    
    Register * r1 = minx_registers_find_register(opc_p->p[0]);
    Register * r2 = minx_registers_find_register(opc_p->p[1]);

    if( r1->value > r2->value ) {
        minx_registers_find_register(akku_register_number)->value = 1;
    }
    else if( r1->value < r2->value ) {
        minx_registers_find_register(akku_register_number)->value = 2;
    }
    else {//( r1->value == r2->value )
        minx_registers_find_register(akku_register_number)->value = 0;
    }
}
