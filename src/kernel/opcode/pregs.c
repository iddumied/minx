#include "kernel/opcodes.h"

/*
 * Command:                 PREGS
 * Parameters:              0
 * Affects Program Pointer: NO 
 */
void opc_pregs_func (void) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("pregs");

    if( minx_config_get(CONF_SRC_DEBUGGING)->b ) {
        unsigned int i;
        for( i = 0; i < register_count ; i++ ) {
            minx_registers_print_register(i);
        }
    }
#endif //DEBUGGING
}
