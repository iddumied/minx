#include "kernel/opcodes.h"

/*
 * Command:                 DROP
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 * Remove element from stack
 *
 */
void opc_drop_func() {
#ifdef DEBUGGING
    /*no explaining here, nothing to explain*/
#endif
    if( stack_is_empty( stack ) ) {
        FATAL_DESC_ERROR("Cannot drop from empty stack!");
    }

    stackpop(stack);
}
