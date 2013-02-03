#include "kernel/opcodes.h"

/*
 * Command:                 PUSH
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
void minx_opc_push_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("reg %"PRIu64, params[0]);
#endif

    stackpush(stack, &(minx_registers_find_register(params[0])->value), VALUE_SIZE) ;
}
