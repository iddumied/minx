#include "kernel.h"

/*
 * -----------------------------------------------------------------------------
 *          static functions prototypes
 * -----------------------------------------------------------------------------
 */

static uint64_t     read_command_parameters(uint16_t *opcode);
static void         run_opcode                  (uint16_t);

/*
 * -----------------------------------------------------------------------------
 *          static variables
 * -----------------------------------------------------------------------------
 */

static Register*                program_pointer;
static int                      program_pointer_manipulated;
static uint64_t                 *params;

static int                      __running__     = 1;
static int                      __exit_code__   = 0;

/*
 * -----------------------------------------------------------------------------
 *          "static" macros
 * -----------------------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 *          visible functions - definitions
 * -----------------------------------------------------------------------------
 */

/**
 * @brief Initialize the kernel
 
 * Public init function 
 *
 * starts the kernel:
 *  - set __kernel_running__ to 1
 *  - init stuff
 *      - init the registers
 *      - init the stack 
 *      - init the command parameters storage
 *
 * The kernel does this init stuff because it is required for the kernel only.
 * There is no reason why another part of minx should initialize these parts!
 */
void minx_kernel_init(void) {
    minx_error_register_shutdown_function(minx_kernel_shutdown);

    __running__     = 1;
    __exit_code__   = 0;

    minx_registers_init();
    program_pointer = minx_registers_find_register(program_pointer_register_number);

    minx_kernel_heap_init();

    stack = empty_stack();

    params = (uint64_t*) malloc(sizeof(uint64_t) * MAX_PARAMETER_COUNT);
}

/**
 * @brief Run the Kernel
 *
 * Public run function
 *
 * - allocates memory for storing the actual opcode
 * - run while __running__
 * - run while current program pointer doesn't point to END_OF_PROGRAM
 * - at END_OF_PROGRAM, if compiled with VERBOSITY and config set, print registers
 * - cleanup the memory, allocated by this function.
 *
 * @return Exit status of the binary
 */
int minx_kernel_run() {
#if (defined DEBUGGING | defined DEBUG)
    minxkerneldbgprint("Starting the kernel\n");
#endif

    uint16_t        *opcode = (uint16_t*) malloc(sizeof(uint16_t));
    uint64_t        next_pp_pos; 

    while( __running__ && program_pointer->value != END_OF_PROGRAM) {

#if (defined DEBUGGING || defined DEBUG)
        fflush(stdout);
#endif // (defined DEBUGGING || defined DEBUG)


        opcode = (uint16_t*) minx_binary_get_at(program_pointer->value, 
                                                OPC_SIZE, 
                                                opcode, 
                                                sizeof(*opcode));

        next_pp_pos = read_command_parameters( opcode );
        run_opcode(*opcode);

        if( !program_pointer_manipulated ) {
            program_pointer->value = next_pp_pos;
        }
        else {
            program_pointer_manipulated = 0;
        }
    }

#if (defined VERBOSITY)
    if( minx_config_get(CONF_PRINT_REGS_AT_EOP)->b ) {
        unsigned int i;
        uint16_t register_count = minx_registers_get_register_count();
        for( i = 0; i < register_count ; i++ ) {
            minx_registers_print_register(i);
        }
    }
#endif //if (defined VERBOSITY)

    return __exit_code__;
}

/**
 * @brief Shutdown the kernel
 *
 * public shutdown function 
 * used to force-shutdown the kernel.
 *
 *  - cleanup 
 *      - free the registers 
 *      - free the stack 
 *      - free the command parameters storage
 *
 * The cleanup must be done here, because if the global shutdown is started from
 * a fatal, after the shutdown() calls, exit() will be called.
 */
void minx_kernel_shutdown() {
    free(params);
    stackdelete(stack);
}

/**
 * @brief Get the current program pointer
 *
 * @return The current program pointer
 */
uint64_t minx_kernel_program_pointer_get(void) {
    return program_pointer->value;
}

/**
 * @brief Manipulate the program pointer
 *
 * @param new_pointer Set the program pointer to this value
 */
void minx_kernel_program_pointer_manipulate(uint64_t new_pointer) {
    program_pointer_manipulated = 1;
    program_pointer->value = new_pointer;
}

/**
 * @brief unset the running variables
 */
void minx_kernel_unset_running_variable(void) {
    __running__ = 0;
}

/**
 * @brief Set the exit status of the kernel
 *
 * @param status Exit status to set
 */
void minx_kernel_set_exit_status(int status) {
    __exit_code__ = status;
}


/*
 * -----------------------------------------------------------------------------
 *          static functions - definitions
 * -----------------------------------------------------------------------------
 */

/**
 * @brief Run a opcide
 * 
 * runs the opcode, passed to the function by finding it in the opc_funcs array.
 *
 * @param opc The opcode to run
 */
static void run_opcode(uint16_t opc) {

#if (defined DEBUGGING | defined DEBUG)

    minxkerneldbgprintf("Running opcode: %"PRIu16"\n", opc);
    
    if(minx_config_get(CONF_SRC_DEBUGGING)->b) {
       printf(MINX_KERNEL_OP_PRINT_PREFIX" %s: ", opcodes[opc].strrep);
    }

    fflush(stdout);
#endif 

    if(!opcodes[opc].opc_func) {
        FATAL_F_ERROR("Tried to execute unknown opcode %"PRIu16"!", opc);
    }
    opcodes[opc].opc_func(params);


#if (defined DEBUGGING | defined DEBUG)
    if(minx_config_get(CONF_SRC_DEBUGGING)->b) {
        printf("\n");
    }
    if ( program_pointer->value != END_OF_PROGRAM ) {
        minxkerneldbgprintf("PROG_POINTER: %"PRIu64"\n", program_pointer->value);
    }
    else {
        minxkerneldbgprint("END OF PROGRAM\n");
    }
#endif 

} // static void run_opcode

/*
 * -----------------------------------------------------------------------------
 *
 *      Argument section - all helpers for opcode arguments 
 *
 * -----------------------------------------------------------------------------
 */

/**
 * @brief Read command parameters for a opcode
 *
 * Helper function for parsing command parameters
 * args:
 *
 * @param n             count of parameters 
 * @param sizes         sizes of the parameters
 *
 * Why
 * ---
 *
 *  Reading the parameters from the binary is always the same procedure:
 *      1) go to (program_pointer + OPC_SIZE )
 *      2) read the value there
 *      3) go to (program_pointer + OPC_SIZE + <size of prev read data>)
 *      4) read the value there
 *      5) go to 3) or ready
 *
 *
 * @param opcode Read parameters of this opcode
 *
 * @return Next program pointer position
 */
static uint64_t read_command_parameters(uint16_t *opcode) {
    uint64_t        next_pos = program_pointer->value + OPC_SIZE;
    unsigned int    i;

    for(i = 0 ; opcodes[*opcode].params[i] || i < MAX_PARAMETER_COUNT; i++ ) {
        params[i] = *((uint64_t*) minx_binary_get_at(   next_pos, 
                                                        opcodes[*opcode].params[i], 
                                                        &params[i], 
                                                        sizeof(uint64_t)));
        next_pos += opcodes[*opcode].params[i];
    }

    return next_pos;
}
