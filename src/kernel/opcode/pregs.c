#include "kernel/opcodes.h"

/*
 * Command:                 PREGS
 * Parameters:              0
 * Affects Program Pointer: NO 
 */
void opc_pregs_func (void) {
#ifdef DEBUGGING
    /* nothing to explain here */

    if( minx_config_get(CONF_SRC_DEBUGGING)->b ) {
        unsigned int i;
        uint16_t register_count = minx_registers_get_register_count();
        for( i = 0; i < register_count ; i++ ) {
            minx_registers_print_register(i);
        }
    }
#endif //DEBUGGING
}
