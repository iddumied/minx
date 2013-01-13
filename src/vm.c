#include "vm.h"

/*
 * static functions prototypes
 * ---------------------------
 */

static void         init_registers              (void);
static Register*    find_register               (uint64_t addr);

static void         run                         (void);

static void         read_1_command_parameter    (unsigned int size1);
static void         read_2_command_parameters   (unsigned int size1, 
                                                 unsigned int size2);

static void         run_opcode              (uint16_t);

static void         opc_nop_func            (void);
static void         opc_call_func           (void);
static void         opc_ret_func            (void);

static void         opc_mov_func            (void);
static void         opc_movi_func           (void);

static void         opc_not_func            (void);
static void         opc_notr_func           (void);

static void         opc_and_func            (void);
static void         opc_andi_func           (void);
static void         opc_andr_func           (void);
static void         opc_andir_func          (void);

static void         opc_or_func             (void);
static void         opc_ori_func            (void);
static void         opc_orr_func            (void);
static void         opc_orir_func           (void);

static void         opc_dec_func            (void);
static void         opc_inc_func            (void);

static void         opc_lsh_func            (void);
static void         opc_rsh_func            (void);

static void         opc_cmp_func            (void);
static void         opc_cmpi_func           (void);
static void         opc_eql_func            (void);
static void         opc_eqli_func           (void);

static void         opc_push_func           (void);
static void         opc_pop_func            (void);
static void         opc_drop_func           (void);

static void         opc_add_func            (void);
static void         opc_addi_func           (void);
static void         opc_addr_func           (void);
static void         opc_addir_func          (void);

static void         opc_jmp_func            (void);
static void         opc_jmpiz_func          (void);
static void         opc_jmpnz_func          (void);
static void         opc_ifzjmp_func         (void);

static void         opc_pstack_func         (void);
static void         opc_pregs_func          (void);
static void         opc_pprog_func          (void);

#ifdef VERBOSITY
static void         print_register          (unsigned int i);
#endif 

/*
 * static variables
 * ----------------
 */

static Register             *   registers       = NULL;
static uint16_t                 register_count  = 0;

static Stack                *   stack           = NULL;
static CommandParameters    *   opc_p           = NULL;

static void ((*opc_funcs[])(void)) = {

    [OPC_NOP]       = opc_nop_func,
    [OPC_CALL]      = opc_call_func,
    [OPC_RET]       = opc_ret_func,

    [OPC_MOV]       = opc_mov_func,
    [OPC_MOVI]      = opc_movi_func,

    [OPC_NOT]       = opc_not_func,
    [OPC_NOTR]      = opc_notr_func,

    [OPC_AND]       = opc_and_func,
    [OPC_ANDI]      = opc_andi_func,
    [OPC_ANDR]      = opc_andr_func,
    [OPC_ANDIR]     = opc_andir_func,

    [OPC_OR]        = opc_or_func,
    [OPC_ORI]       = opc_ori_func,
    [OPC_ORR]       = opc_orr_func,
    [OPC_ORIR]      = opc_orir_func,

    [OPC_DEC]       = opc_dec_func,
    [OPC_INC]       = opc_inc_func,

    [OPC_LSH]       = opc_lsh_func,
    [OPC_RSH]       = opc_rsh_func,

    [OPC_CMP]       = opc_cmp_func,
    [OPC_CMPI]      = opc_cmpi_func, 
    [OPC_EQL]       = opc_eql_func,
    [OPC_EQLI]      = opc_eqli_func,

    [OPC_PUSH]      = opc_push_func,
    [OPC_POP]       = opc_pop_func,
    [OPC_DROP]      = opc_drop_func,

    [OPC_ADD]       = opc_add_func,
    [OPC_ADDI]      = opc_addi_func,
    [OPC_ADDR]      = opc_addr_func,
    [OPC_ADDIR]     = opc_addir_func,

    [OPC_JMP]       = opc_jmp_func,
    [OPC_JMPIZ]     = opc_jmpiz_func,
    [OPC_JMPNZ]     = opc_jmpnz_func,
    [OPC_IFZJMP]    = opc_ifzjmp_func,

    [OPC_PSTACK]    = opc_pstack_func,
    [OPC_PREGS]     = opc_pregs_func,
    [OPC_PPROG]     = opc_pprog_func
};

/*
 * "static" macros 
 * ---------------
 */ 

/*
 * These macros are always valid, also if the register_map was reordered,
 * because there are in a row and defined as so.
 */
#define program_pointer                 (registers[0x0000].value)
#define akku                            (registers[0x0002].value)
#define statusregister                  (registers[0x0003].value)

#define register_exists(addr)           (register_count >= addr)
#define program_pointer_is(val)         (program_pointer == val)

/*
 * Functions
 * ---------
 */

void minx_vm_run() {
#if (defined DEBUGGING | defined DEBUG)
    minxvmdbgprint("Starting\n");
#endif

    /* alloc standard registers */
    init_registers();

    stack = empty_stack();
    opc_p = (CommandParameters*) malloc( sizeof(*opc_p) );

    run();

    free(registers);
    free(opc_p);
    stackdelete(stack);
}

/* 
 * static functions 
 * ----------------
 */

static void init_registers() {
#if (defined DEBUGGING | defined DEBUG)
    minxvmdbgprintf("init %i registers\n", MAX_REGISTERS);
#endif

    registers = (Register*) malloc( sizeof(Register) * MAX_REGISTERS);

    /* abuse register_map->reg_count as counter */
    for(    register_count = 0x0000 ; 
            register_count < MAX_REGISTERS; 
            register_count++ ) {

        registers[register_count].value = 0x00;
    }
}

/*
 * find a register in the register_map by it's address.
 *
 * @param addr address of the register. Here is 64 Bit required, because
 * parameters are read into 64 bit memory, masking is done in _this_ function
 * and nowhere else. So if a opcode function reads it's parameters, they are
 * saved in 64 bit and passed to this function if necessary.
 */
static Register* find_register(uint64_t addr) {
#if (defined DEBUGGING | defined DEBUG)
    minxvmdbgprintf("find register: %"PRIu64"\n", addr);
#endif 

    if( !register_exists(addr) ) {
        FATAL_DESC_ERROR("Register does not exist");
    }
    return &registers[ addr | REGISTER_ADDRESS_SIZE ]; 
}

/*
 * run function 
 *
 * runs the vm.  
 */
static void run() {
#if (defined DEBUGGING | defined DEBUG)
    minxvmdbgprint("run");
#endif //DEBUGGING

    uint16_t *opcode;

    while( !program_pointer_is(END_OF_PROGRAM) ) {

#if (defined DEBUGGING || defined DEBUG)
        fflush(stdout);
#endif // (defined DEBUGGING || defined DEBUG)
        opcode = minx_binary_get_at(program_pointer, OPC_SIZE, opcode);
        run_opcode(*opcode);
        free(opcode);
    }

#if (defined VERBOSITY)
    if( minx_config_get(CONF_PRINT_REGS_AT_EOP)->b ) {
        unsigned int i;
        for( i = 0; i < register_count ; i++ ) {
            print_register(i);
        }
    }
#endif //if (defined VERBOSITY)
} //static void run()

/*
 * runs the opcode, passed to the function by finding it in the opc_funcs array.
 *
 * @param cmd the opcode to run.
 *
 */
static void run_opcode(uint16_t cmd) {
#if (defined DEBUGGING | defined DEBUG)
    minxvmdbgprintf("Running opcode: %"PRIu16"\n", cmd);
    fflush(stdout);
#endif 

    void (*opc_func)(void) = opc_funcs[cmd];
    if( opc_func == NULL ) {
        FATAL_DESC_ERROR("Tried to execute unknown opcode!");
    }
    opc_func();

#if (defined DEBUGGING | defined DEBUG)
    if ( program_pointer != END_OF_PROGRAM ) {
        minxvmdbgprintf("[minx][vm]:\tPROG_POINTER: %"PRIu64"\n", program_pointer);
    }
    else {
        minxvmdbgprint("[minx][vm]: END OF PROGRAM\n");
    }
#endif 
}

/*
 * Helper function for parsing command parameters
 * args:
 *
 * @param cp            storage for the result 
 * @param size1         size of argument 1
 *
 * Why
 * ---
 *
 *  Have a look at the description for read_2_command_parameters() function.
 *
 */
static void read_1_command_parameter(unsigned int size1) {
    uint64_t ptr1_location = program_pointer + OPC_SIZE;
    opc_p->p1 = *((uint64_t *) minx_binary_get_at( ptr1_location, size1, &opc_p->p1));
}

/*
 * Helper function for parsing command parameters
 * args:
 *
 * @param cp            storage for the result 
 * @param size1         size of argument 1, used as offset, too
 * @param size2         size of argument 2
 *
 * Why
 * ---
 *
 *  Reading the parameters from the binary is always the same procedure:
 *      - go to (program_pointer + OPC_SIZE )
 *      - read the value there, it could be a constant or a pointer, doesn't
 *      matter yet
 *      - go to (program_pointer + OPC_SIZE + <size of prev read data>)
 *      - read the value there, it could be a constant or a pointer, doesn't
 *      matter yet
 *
 *  and then use this data. The upper part is done by this helper function, to
 *  simplify the work of the opc_ functions.
 */
static void read_2_command_parameters(unsigned int size1, unsigned int size2) {

    uint64_t ptr1_location = program_pointer + OPC_SIZE;
    uint64_t ptr2_location = program_pointer + OPC_SIZE + size1;

    opc_p->p1 = *((uint64_t *) minx_binary_get_at( ptr1_location, size1, &opc_p->p1));
    opc_p->p2 = *((uint64_t *) minx_binary_get_at( ptr2_location, size2, &opc_p->p2));
}

/*
 *
 * =============================================================================
 *                          Op codes implementations
 * =============================================================================
 *
 *
 * (Almost) each opcode has to modify the program pointer to the byte after 
 * the opcode.
 *
 * Explanation:
 *  mov 0x00FF, 0x00FE
 * takes 
 *    16 Bit 
 *  + 64 Bit
 *  + 64 Bit
 *  --------
 *   144 Bit = 18 Byte
 *
 * so the program pointer has to be set to 
 *  program_pointer = program_pointer + 18 Byte
 *
 * If the opcode does affect the program pointer, this has not to be done!
 *
 */

/*
 * Command:                 NOP
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_nop_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("nop");
#endif
    program_pointer += (OPC_SIZE);
}

/*
 * Command:                 CALL
 * Parameters:              1, program address
 * Affects Program Pointer: YES
 *
 * get current program_pointer, add enough for the next opcode, push it to stack,
 * set the program_pointer to the value of the argument of the opc.
 */
static void opc_call_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("call");
#endif
    read_1_command_parameter(PROGRAM_ADDRESS_SIZE);
    /*
     * push to stack
     *
     * push the address of the next opcode to the stack,
     * the address has size (PROGRAM_ADDRESS_SIZE) (in this case this is 8 Byte)
     */
    stackpush(  stack,
                (void*)(program_pointer + OPC_SIZE + PROGRAM_ADDRESS_SIZE), 
                (size_t)PROGRAM_ADDRESS_SIZE);
    program_pointer = opc_p->p1;
}

/*
 * Command:                 RET
 * Parameters:              0
 * Affects Program Pointer: YES
 *
 *
 */
static void opc_ret_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("ret");
#endif
    if (stack_is_empty(stack))
        FATAL_DESC_ERROR("Cannot RET, stack is empty!");
     
    program_pointer = (uint64_t) stackpop(stack);
}

/*
 * Command:                 MOV 
 * Parameters:              2: register-adress register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_mov_func() {
    read_2_command_parameters(REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("mov", "from: %"PRIu64", to: %"PRIu64, opc_p->p2, opc_p->p1);
#endif 

    find_register(opc_p->p1)->value = find_register(opc_p->p2)->value;
    program_pointer += (OPC_SIZE + REGISTER_ADDRESS_SIZE + REGISTER_ADDRESS_SIZE);
}

/*
 * Command:                 MOVI
 * Parameters:              2: register-address value
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_movi_func() {
    read_2_command_parameters(REGISTER_ADDRESS_SIZE, VALUE_SIZE);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("movi", "val: %"PRIu64", to: %"PRIu64, opc_p->p2, opc_p->p1);
#endif 

    find_register(opc_p->p1)->value = opc_p->p2;

    program_pointer += ( OPC_SIZE + REGISTER_ADDRESS_SIZE + VALUE_SIZE );
}

/*
 * Command:                 NOT 
 * Parameters:              1: register-address 
 * Affects Program Pointer: NO
 *
 * Result in akku
 */
static void opc_not_func() {
    read_1_command_parameter(REGISTER_ADDRESS_SIZE);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("not", "register: %"PRIu64, opc_p->p1);
#endif 

    akku = ! find_register(opc_p->p1)->value;

    program_pointer += ( OPC_SIZE + REGISTER_ADDRESS_SIZE );
}

/*
 * Command:                 NOTR
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 * Result in same register
 */
static void opc_notr_func() {
    read_1_command_parameter(REGISTER_ADDRESS_SIZE);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("not", "register: %"PRIu64, opc_p->p1);
#endif 

    find_register(opc_p->p1)->value = ! find_register(opc_p->p1)->value;
 
    program_pointer += ( OPC_SIZE + REGISTER_ADDRESS_SIZE );
}

/*
 * Command:                 AND
 * Parameters:              2: register-address register-address
 * Affects Program Pointer: NO
 *
 * result in akku
 */
static void opc_and_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("and");
#endif

    read_2_command_parameters(REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE);
    akku = find_register(opc_p->p1)->value & find_register(opc_p->p2)->value;

    program_pointer += ( OPC_SIZE + REGISTER_ADDRESS_SIZE + REGISTER_ADDRESS_SIZE );
}

/*
 * Command:                 ANDI
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 * Result in akku
 */
static void opc_andi_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("andi");
#endif

    read_2_command_parameters(REGISTER_ADDRESS_SIZE, VALUE_SIZE);
    akku = find_register(opc_p->p1)->value & opc_p->p2;

    program_pointer += ( OPC_SIZE + REGISTER_ADDRESS_SIZE + VALUE_SIZE);
}

/*
 * Command:                 ANDR
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 * Result in first register
 */
static void opc_andr_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("andr");
#endif

    read_2_command_parameters(REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE);
    find_register(opc_p->p1)->value &= find_register(opc_p->p2)->value;

    program_pointer += ( OPC_SIZE + REGISTER_ADDRESS_SIZE + REGISTER_ADDRESS_SIZE );
}

/*
 * Command:                 ANDIR
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 * Result in first register
 */
static void opc_andir_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("andir");
#endif

    read_2_command_parameters(REGISTER_ADDRESS_SIZE, VALUE_SIZE);
    find_register(opc_p->p1)->value &= opc_p->p2;

    program_pointer += ( OPC_SIZE + REGISTER_ADDRESS_SIZE + VALUE_SIZE);
}


/*
 * Command:                 OR
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 * Result in akku
 */
static void opc_or_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("or");
#endif

    read_2_command_parameters(REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE);
    akku = find_register(opc_p->p1)->value | find_register(opc_p->p2)->value;

    program_pointer += ( OPC_SIZE + REGISTER_ADDRESS_SIZE + REGISTER_ADDRESS_SIZE );
}

/*
 * Command:                 ORI 
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_ori_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("ori");
#endif

    read_2_command_parameters(REGISTER_ADDRESS_SIZE, VALUE_SIZE);
    akku = find_register(opc_p->p1)->value | opc_p->p2;

    program_pointer += ( OPC_SIZE + REGISTER_ADDRESS_SIZE + VALUE_SIZE);
}

/*
 * Command:                 ORR
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_orr_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("orr");
#endif

    read_2_command_parameters(REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE);
    find_register(opc_p->p1)->value |= find_register(opc_p->p2)->value;

    program_pointer += ( OPC_SIZE + REGISTER_ADDRESS_SIZE + REGISTER_ADDRESS_SIZE);
}

/*
 * Command:                 ORIR
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_orir_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("orir");
#endif

    read_2_command_parameters(REGISTER_ADDRESS_SIZE, VALUE_SIZE);
    find_register(opc_p->p1)->value |= opc_p->p2;

    program_pointer += ( OPC_SIZE + REGISTER_ADDRESS_SIZE + VALUE_SIZE);
}

/*
 * Command:                 DEC
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_dec_func() {
    read_1_command_parameter(REGISTER_ADDRESS_SIZE);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("dec", "reg: %"PRIu64, opc_p->p1);
#endif 

    if( find_register(opc_p->p1)->value == 0x0000 ) {
        setbit(statusregister, OVERFLOW_BIT);
    }

    find_register(opc_p->p1)->value--;
    
    program_pointer += (OPC_SIZE + REGISTER_ADDRESS_SIZE);
}

/*
 * Command:                 INC
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_inc_func() {
    read_1_command_parameter(REGISTER_ADDRESS_SIZE);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("inc", "reg: %"PRIu64, opc_p->p1);
#endif 

    if( find_register(opc_p->p1)->value == 0xFFFF ) {
        setbit(statusregister, OVERFLOW_BIT);
    }

    find_register(opc_p->p1)->value++;
    
    program_pointer += (OPC_SIZE + REGISTER_ADDRESS_SIZE);
}

/*
 * Command:                 LSH
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_lsh_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("lsh");
#endif
    read_1_command_parameter(REGISTER_ADDRESS_SIZE);
    Register *r = find_register(opc_p->p1);
    r->value = r->value<<1;
    
    program_pointer += (OPC_SIZE + REGISTER_ADDRESS_SIZE);
}

/*
 * Command:                 RSH
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_rsh_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("rsh");
#endif
    read_1_command_parameter(REGISTER_ADDRESS_SIZE);
    Register *r = find_register(opc_p->p1);
    r->value = r->value>>1;
    
    program_pointer += (OPC_SIZE + REGISTER_ADDRESS_SIZE);
}

/*
 * Command:                 CMP 
 * Parameters:              2: register-address register-address
 * Affects Program Pointer: NO
 *
 * compare, 
 * result in akku, 
 * 1 if first is bigger, 
 * 2 if second is bigger, 
 * zero if equal
 */
static void opc_cmp_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("cmp");
#endif 
    
    read_2_command_parameters(REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE);
    Register * r1 = find_register(opc_p->p1);
    Register * r2 = find_register(opc_p->p2);

    if( r1->value > r2->value ) {
        akku = 1;
    }
    else if( r1->value < r2->value ) {
        akku = 2;
    }
    else {//( r1->value == r2->value )
        akku = 0;
    }

    program_pointer += (OPC_SIZE + REGISTER_ADDRESS_SIZE + REGISTER_ADDRESS_SIZE);
}

/*
 * Command:                 CMPI 
 * Parameters:              1: register-address value 
 * Affects Program Pointer: NO
 *
 * compare, 
 * result in akku, 
 * 1 if first is bigger, 
 * 2 if second is bigger, 
 * zero if equal
 */
static void opc_cmpi_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("cmpi");
#endif 
    
    read_2_command_parameters(REGISTER_ADDRESS_SIZE, VALUE_SIZE);
    Register * r1 = find_register(opc_p->p1);

    if(r1->value > opc_p->p2) {
        akku = 1;
    }
    else if(r1->value < opc_p->p2) {
        akku = 2;
    }
    else {//( r1->value == r2->value )
        akku = 0;
    }

    program_pointer += (OPC_SIZE + REGISTER_ADDRESS_SIZE + VALUE_SIZE);
}

/*
 * Command:                 EQL 
 * Parameters:              1: register-address register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_eql_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("eql");
#endif 

    read_2_command_parameters(REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE);
    akku = find_register(opc_p->p1)->value == find_register(opc_p->p1)->value;

    program_pointer += (OPC_SIZE + REGISTER_ADDRESS_SIZE + REGISTER_ADDRESS_SIZE);
}

/*
 * Command:                 EQLI 
 * Parameters:              1: register-address value
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_eqli_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("eqli");
#endif 

    read_2_command_parameters(REGISTER_ADDRESS_SIZE, VALUE_SIZE);
    akku = find_register(opc_p->p1)->value == opc_p->p2;

    program_pointer += (OPC_SIZE + REGISTER_ADDRESS_SIZE + VALUE_SIZE);
}

/*
 * Command:                 PUSH
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_push_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("push");
#endif
    read_1_command_parameter(REGISTER_ADDRESS_SIZE);
    stackpush(stack, &(find_register(opc_p->p1)->value), VALUE_SIZE) ;
    
    program_pointer += (OPC_SIZE + REGISTER_ADDRESS_SIZE);
}

/*
 * Command:                 POP
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_pop_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("pop");
#endif
    if( stack_is_empty( stack ) ) {
        FATAL_DESC_ERROR("Cannot pop from empty stack!");
    }

    read_1_command_parameter(REGISTER_ADDRESS_SIZE);
    find_register(opc_p->p1)->value = *((uint64_t*)stackpop(stack));
    
    program_pointer += (OPC_SIZE + REGISTER_ADDRESS_SIZE);
}

/*
 * Command:                 DROP
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 * Remove element from stack
 *
 */
static void opc_drop_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("drop");
#endif
    if( stack_is_empty( stack ) ) {
        FATAL_DESC_ERROR("Cannot drop from empty stack!");
    }

    stackpop(stack);
    program_pointer += (OPC_SIZE);
}

/*
 * Command:                 ADD
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 * Result in akku
 */
static void opc_add_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("add");
#endif
    read_2_command_parameters(REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE);
    akku = find_register(opc_p->p1)->value + find_register(opc_p->p2)->value;

    program_pointer += (OPC_SIZE + REGISTER_ADDRESS_SIZE + REGISTER_ADDRESS_SIZE);
}

/*
 * Command:                 ADDI
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 * Result in akku
 */
static void opc_addi_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("addi");
#endif
    read_2_command_parameters(REGISTER_ADDRESS_SIZE, VALUE_SIZE);
    akku = find_register(opc_p->p1)->value + opc_p->p2;

    program_pointer += (OPC_SIZE + REGISTER_ADDRESS_SIZE + VALUE_SIZE);
}

/*
 * Command:                 ADDR
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 * Result in register
 */
static void opc_addr_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("addr");
#endif
    read_2_command_parameters(REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE);
    find_register(opc_p->p1)->value += find_register(opc_p->p2)->value;

    program_pointer += (OPC_SIZE + REGISTER_ADDRESS_SIZE + REGISTER_ADDRESS_SIZE);
}

/*
 * Command:                 ADDIR
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 * Result in register
 */
static void opc_addir_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("addir");
#endif
    read_2_command_parameters(REGISTER_ADDRESS_SIZE, VALUE_SIZE);
    find_register(opc_p->p1)->value += opc_p->p2;

    program_pointer += (OPC_SIZE + REGISTER_ADDRESS_SIZE + VALUE_SIZE);
}

/*
 * Command:                 JMP
 * Parameters:              1: address
 * Affects Program Pointer: YES
 *
 *
 */
static void opc_jmp_func() {
    read_1_command_parameter(PROGRAM_ADDRESS_SIZE);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("jmp", "to %"PRIu64, opc_p->p1);
#endif 

    if( minx_binary_exists_at(opc_p->p1) || opc_p->p1 == END_OF_PROGRAM) {
        program_pointer = opc_p->p1;
    }
    else {
        FATAL_DESC_ERROR("Cannot jump");
    }
}

/*
 * Command:                 JMPIZ
 * Parameters:              2: register-address, address
 * Affects Program Pointer: YES
 *
 *
 */
static void opc_jmpiz_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("jmpiz");
#endif
    read_2_command_parameters(REGISTER_ADDRESS_SIZE, PROGRAM_ADDRESS_SIZE);
    if( minx_binary_exists_at(opc_p->p2) || opc_p->p2 == END_OF_PROGRAM) {
        if( find_register(opc_p->p1)->value == 0x0000 ) {
            program_pointer = opc_p->p2;
        }
        else {
            program_pointer += ( OPC_SIZE + REGISTER_ADDRESS_SIZE + PROGRAM_ADDRESS_SIZE );
        }
    }
    else {
        FATAL_DESC_ERROR("Cannot jump");
    }
}

/*
 * Command:                 JMPNZ
 * Parameters:              2: register-address, address
 * Affects Program Pointer: YES
 *
 *
 */
static void opc_jmpnz_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("jmpnz");
#endif
    read_2_command_parameters(REGISTER_ADDRESS_SIZE, PROGRAM_ADDRESS_SIZE);
    if( minx_binary_exists_at(opc_p->p2) || opc_p->p2 == END_OF_PROGRAM) {
        if( find_register(opc_p->p1)->value != 0x0000 ) {
            program_pointer = opc_p->p2;
        }
        else {
            program_pointer += ( OPC_SIZE + REGISTER_ADDRESS_SIZE + PROGRAM_ADDRESS_SIZE );
        }
    } 
    else {
        FATAL_DESC_ERROR("Cannot jump");
    }
}

/*
 * Command:                 IFZJMP
 * Parameters:              2: address, address 
 * Affects Program Pointer: YES
 *
 * if akku is zero then jump else jump
 */
static void opc_ifzjmp_func() {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("ifzjmp");
#endif
    read_2_command_parameters(PROGRAM_ADDRESS_SIZE, PROGRAM_ADDRESS_SIZE);
    if( (minx_binary_exists_at(opc_p->p1) || opc_p->p1 == END_OF_PROGRAM) && 
        (minx_binary_exists_at(opc_p->p2) || opc_p->p2 == END_OF_PROGRAM)) {

        if( akku == 0x0000 ) {
            program_pointer = opc_p->p1;
        }
        else {
            program_pointer = opc_p->p2;
        }
    } 
    else {
        FATAL_DESC_ERROR("Cannot jump");
    }
}

/*
 * Command:                 PSTACK
 * Parameters:              0
 * Affects Program Pointer: NO 
 */
static void opc_pstack_func (void) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("pstack");
    if( minx_config_get(CONF_SRC_DEBUGGING)->b ) {
        stack_print_binary(stack);
    }

#endif //DEBUGGING
    program_pointer += OPC_SIZE;
}

/*
 * Command:                 PREGS
 * Parameters:              0
 * Affects Program Pointer: NO 
 */
static void opc_pregs_func (void) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("pregs");

    if( minx_config_get(CONF_SRC_DEBUGGING)->b ) {
        unsigned int i;
        for( i = 0; i < register_count ; i++ ) {
            print_register(i);
        }
    }
#endif //DEBUGGING
    program_pointer += OPC_SIZE;
}

/*
 * Command:                 PPROG
 * Parameters:              0
 * Affects Program Pointer: NO 
 */
static void opc_pprog_func (void) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("pprog");

    if( minx_config_get(CONF_SRC_DEBUGGING)->b ) {
        minx_binary_print();
    }

#endif //DEBUGGING
    program_pointer += OPC_SIZE;
}

#if (defined VERBOSITY | defined DEBUGGING)
static void print_register(unsigned int i) {
    printf( MINX_VM_PRINT_PREFIX"[register][%03i] = %"PRIu64"\n", i, registers[i].value );
}
#endif //(defined VERBOSITY | defined DEBUGGING)

/*
 * "static" macros undef  
 * ---------------------
 */ 
#undef program_pointer
#undef akku
#undef statusregister

#undef register_exists
#undef program_pointer_is
