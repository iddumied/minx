#include "kernel/opcodes.h"

/**
 * @brief Opcode CMP 
 *
 * Command:                 CMP 
 * Parameters:              2: register-address register-address
 * Affects Program Pointer: NO
 *
 * compare, 
 *
 * result in akku_register_number, 
 * 1 if first is bigger, 
 * 2 if second is bigger, 
 * zero if equal
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_cmp_func(uint64_t *params) {

#ifdef DEBUGGING
    EXPLAIN_OPCODE("reg %"PRIu64" and reg %"PRIu64, params[0], params[1]);
#endif 
    
    Register * r1 = minx_registers_find_register(params[0]);
    Register * r2 = minx_registers_find_register(params[1]);

    Register *akku = minx_registers_find_register(akku_register_number);

    if( r1->value > r2->value ) {
        akku->value = 1;
    }
    else if( r1->value < r2->value ) {
        akku->value = 2;
    }
    else {//( r1->value == r2->value )
        akku->value = 0;
    }
}
