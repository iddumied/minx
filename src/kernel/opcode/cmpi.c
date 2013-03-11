#include "kernel/opcodes.h"

/**
 * @brief Opcode CMPI 
 *
 * Comm<=>:                 CMPI 
 * Parameters:              1: register-address value 
 * Affects Program Pointer: NO
 *
 * compare, 
 *
 * result in minx_registers_find_register(akku_register_number)->value, 
 * 1 if first is bigger, 
 * 2 if second is bigger, 
 * zero if equal
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_cmpi_func(uint64_t *params) {
    Register * r1 = minx_registers_find_register(params[0]);
    Register *akku = minx_registers_find_register(akku_register_number);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
            "R%"PRIu64" (%#010"PRIx64") <=> %#010"PRIx64, 
            "R%"PRIu64" (%"PRIu64") <=> %"PRIu64, 
            params[0], r1->value, params[1]);
#endif 
    
    if(r1->value > params[1]) {
        akku->value = 1;
    }
    else if(r1->value < params[1]) {
        akku->value = 2;
    }
    else {//( r1->value == r2->value )
        akku->value = 0;
    }

}
