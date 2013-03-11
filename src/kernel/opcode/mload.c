#include "kernel/opcodes.h"

/*
 * Command:                 MLOAD
 * Parameters:              1: heap-address
 * Affects Program Pointer: NO
 *
 */
void minx_opc_mload_func(uint64_t *params) {
    Register *akku  = minx_registers_find_register(akku_register_number);
    Register *r1    = minx_registers_find_register(params[0]);
    
    uint64_t heap_size      = minx_kernel_heap_get_size(r1->value);
    char    *module_name    = (char*) malloc(heap_size * sizeof(char) );

    if(module_name) {
        minx_kernel_heap_read(r1->value, 0x00, heap_size, (uint64_t*)module_name);

#ifdef DEBUGGING
    EXPLAIN_OPCODE("Try to load \"%s\"", module_name);
#endif

        akku->value = minx_kernel_module_load(module_name);
        /*
         * do not free the module_name as this gets used by the kernel.
         */
    }
    else {
        Register *status = minx_registers_find_register(statusregister);
        setbit(status->value, MODULE_ERROR_LOAD);
    }
}
