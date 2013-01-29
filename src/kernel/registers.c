#include "registers.h"

static Register             *   registers       = NULL;
static uint16_t                 register_count  = 0;

#define register_exists(addr)   (register_count >= addr)

/*
 * init registers 
 *
 * allocate memory for the registers, set all registers to 0x00 and set the
 * register-counter.
 */
void minx_registers_init() {

    minx_error_register_shutdown_function(minx_registers_shutdown);

#if (defined DEBUGGING | defined DEBUG)
    minxvpudbgprintf("init %i registers\n", MAX_REGISTERS);
#endif

    registers = (Register*) malloc( sizeof(Register) * MAX_REGISTERS);

    /* abuse register_map->reg_count as counter */
    for(    register_count = 0x0000 ; 
            register_count < MAX_REGISTERS; 
            register_count++ ) {

        registers[register_count].value = 0x00;
    }
}


void minx_registers_shutdown(void) {
    free(registers);
}

/*
 * find a register in the register_map by it's address. If the requested
 * register does not exist, fail.
 */
Register* minx_registers_find_register(uint64_t addr) {
#if (defined DEBUGGING | defined DEBUG)
    minxvpudbgprintf("find register: %"PRIu64"\n", addr);
#endif 

    if( !register_exists(addr) ) {
        FATAL_F_ERROR("Register %"PRIu64" does not exist!", addr);
    }
    return &registers[ addr ]; 
}

/*
 * -----------------------------------------------------------------------------
 *          optional functions - definitions
 * -----------------------------------------------------------------------------
 */

#if (defined VERBOSITY | defined DEBUGGING)
void minx_registers_print_register(unsigned int i) {
    if( minx_config_get(CONF_HEX)->b ) {
        printf( MINX_KERNEL_REGISTER_PREFIX"[%03i] = %#010"PRIx64"\n", 
                i, minx_registers_find_register(i)->value );
    }
    else {
        printf( MINX_KERNEL_REGISTER_PREFIX"[%03i] = %"PRIu64"\n", 
                i, minx_registers_find_register(i)->value );
    }
}
#endif //(defined VERBOSITY | defined DEBUGGING)

