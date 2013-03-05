#include "registers.h"

/*
 * static functions
 */

static RegisterStackPage* new_reg_stack_page(void);

/*
 * static variables
 */

static Register             *   registers       = NULL;
static uint16_t                 register_count  = 0;

#define register_exists(addr)   (register_count >= addr)

/**
 * @brief Register stack for pushing all registers at once
 */
static Stack                *   register_stack;

/**
 * @brief Initialize the registers of the kernel
 *
 * init registers 
 *
 * allocate memory for the registers, set all registers to 0x00 and set the
 * register-counter.
 */
void minx_registers_init() {

    minx_error_register_shutdown_function(minx_registers_shutdown);

#if (defined DEBUGGING | defined DEBUG)
    minxkerneldbgprintf("init %i registers\n", MAX_REGISTERS);
#endif

    registers = (Register*) malloc( sizeof(Register) * MAX_REGISTERS);

    /* abuse register_map->reg_count as counter */
    for(    register_count = 0x0000 ; 
            register_count < MAX_REGISTERS; 
            register_count++ ) {

        registers[register_count].value = 0x00;
    }

    /* initialize the register_stack */
    register_stack = empty_stack();
}

/**
 * @brief Shutdown the registers of the kernel
 */
void minx_registers_shutdown(void) {
    free(registers);
}

/**
 * @brief get the number of registers
 *
 * @return The number of registers
 */
uint16_t minx_registers_get_register_count(void) {
    return register_count;
}

/**
 * @brief Find a register by its address
 *
 * find a register in the register_map by it's address. If the requested
 * register does not exist, fail.
 *
 * @param addr The register adress
 *
 * @return A pointer to the found register
 */
Register* minx_registers_find_register(uint64_t addr) {
#if (defined DEBUGGING | defined DEBUG)
    minxkerneldbgprintf("find register: %"PRIu64"\n", addr);
#endif 

    if( !register_exists(addr) ) {
        FATAL_F_ERROR("Register %"PRIu64" does not exist!", addr);
    }
    return &registers[ addr ]; 
}

/**
 * @brief Push all registers except 0x00 to the register stack
 *
 * Sets all registers except 0x00 (program pointer) to 0x00 after storing the
 * current data on the register stack.
 *
 * @return 0 if failed, else 1
 */
int minx_registers_push(void) {
    RegisterStackPage *rsp = new_reg_stack_page();

    uint16_t i;
    for(i = 1; i < register_count; i++) {
        rsp->values[i-1] = registers[i]->value;
        registers[i]->value = 0x00;
    }

    stackpush(register_stack, rsp->values, (sizeof(uint64_t)*(register_count-1)));
    free(rsp);

    return 1;
}

/**
 * @brief Pop all registers except 0x00 from the register stack
 *
 * Overrides all registers except the program pointer
 *
 * @return 0 if failed, else 1
 */
int minx_registers_pop(void) {
    RegisterStackPage *rsp = new_reg_stack_page();
    rsp->values = (uint64_t*)stackpop(register_stack);

    uint16_t i;
    for( i = 1; i < register_count; i++) {
        registers[i]->value = rsp->values[i-1];
    }

    free(rsp);

    return 1;
}

/*
 * -----------------------------------------------------------------------------
 *          optional functions - definitions
 * -----------------------------------------------------------------------------
 */

#if (defined VERBOSITY | defined DEBUGGING)
/**
 * @brief Print a register
 *
 * @param i The register number
 */
void minx_registers_print_register(unsigned int i) {

#define r_val minx_registers_find_register(i)->value

    /*
     * if not print registers with value == 0, return.
     */
    if( minx_config_get(CONF_PRINT_REGS_AT_EOP_WITHOUT_ZEROVALREGS)->b && r_val == 0x00 ) {
        return;
    }

    /*
     * else 
     */
    if( minx_config_get(CONF_HEX)->b ) {
        printf( MINX_KERNEL_REGISTER_PREFIX"[0x%02x] = %#010"PRIx64"\n", 
                i, r_val);
    }
    else {
        printf( MINX_KERNEL_REGISTER_PREFIX"[%03i] = %"PRIu64"\n", i, r_val);
    }
#undef r_val

}
#endif //(defined VERBOSITY | defined DEBUGGING)


/**
 * @brief allocate a new RegisterStackPage or fail
 *
 * @return RegisterStackPage* to new RegisterStackPage
 */
static RegisterStackPage* new_reg_stack_page(void) {
    size_t size = sizeof(RegisterStackPage) + (sizeof(uint64_t) * (register_count-1));
    RegisterStackPage* rsp = (RegisterStackPage*) malloc(size);

    if(rsp == NULL) 
        FATAL_F_ERROR("Could not allocate %zu Bytes for RegisterStackPage", size);

    return rsp;
}
