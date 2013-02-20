#include "kernel/opcodes.h"

/*
 * Command:                 MLOAD
 * Parameters:              1: heap-address
 * Affects Program Pointer: NO
 *
 */
void minx_opc_mload_func(uint64_t *params) {
    Register *akku = minx_registers_find_register(akku_register_number);
    
    uint64_t heap_size      = minx_kernel_heap_get_size(params[0]);
    uint64_t *module_name   = (uint64_t*) malloc(heap_size * sizeof(char) );

    if(module_name) {
        minx_kernel_heap_read(params[0], 0x00, heap_size, module_name);
        akku->value = minx_kernel_module_load( (char*)module_name );
        free(module_name);
    }
    else {
        Register *status = minx_registers_find_register(statusregister);
        setbit(status->value, MODULE_ERROR_LOAD);
    }
}
