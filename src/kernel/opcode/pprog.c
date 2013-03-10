#include "kernel/opcodes.h"

/**
 * @brief Opcode PPROG
 *
 * Command:                 PPROG
 * Parameters:              0
 * Affects Program Pointer: NO 
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_pprog_func(uint64_t *params) {

#ifdef DEBUGGING
    /* nothing to explain here */

    if( minx_config_get(CONF_SRC_DEBUGGING)->b ) {
        minx_binary_print();
    }

#endif //DEBUGGING
}
