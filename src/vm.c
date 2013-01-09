#include "vm.h"

/*
 * static functions prototypes
 * ---------------------------
 */

static void         init_registers              (void);
static Register*    new_register                (uint64_t addr);
static Register*    find_register               (uint64_t addr) {

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

static void         opc_startat_func        (void);

/* NOT SUPPORTED YET
static void         opc_lnreg_func          (void);
static void         opc_lpreg_func          (void);

static void         opc_getp_func           (void);
static void         opc_getpa_func          (void);
*/

/*
 * static variables
 * ----------------
 */

static RegisterMap          *   register_map    = NULL;

//static Stack                *   stack           = NULL;
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
/*
    [0x60] = opc_lnreg_func,
    [0x61] = opc_lpreg_func,

    [0x70] = opc_getp_func,
    [0x71] = opc_getpa_func,
    [0x72] = opc_setp_func,
*/
};

/*
 * "static" macros 
 * ---------------
 */ 

/*
 * These macros are always valid, also if the register_map was reordered,
 * because there are in a row and defined as so.
 */
#define program_pointer                 (register_map->registers[0x0000]->value)
#define akku                            (register_map->registers[0x0002]->value)
#define statusregister                  (register_map->registers[0x0003]->value)

#define register_exists(addr)           (register_map->reg_count >= addr)
#define program_pointer_is(val)         (program_pointer == val)

/*
 * Functions
 * ---------
 */

void minx_vm_run() {
    /* alloc standard registers */
    init_registers();

    stack = empty_stack();
    opc_p = (CommandParameters*) malloc( sizeof(*opc_p) );

    run();

    free(register_map);
    free(opc_p);
    stackdelete(stack);
}

/* 
 * static functions 
 * ----------------
 */

static void init_registers() {
#define DEF_REGS (DEFAULT_REGISTER_CNT+DEFAULT_ADDITIONAL_REGISTERS)

    register_map = (RegisterMap*) malloc( sizeof(RegisterMap) + (DEF_REGS)*(sizeof(Register*)));

    /* abuse register_map->reg_count as counter */
    for(    register_map->reg_count = 0x00 ; 
            register_map->reg_count < (DEF_REGS); 
            register_map->reg_count++ ) {

        register_map->registers[register_map->reg_count] = new_register(register_map->reg_count);
    }

    /* map is sorted, reg_count is set */

#undef DEF_REGS
}

static Register* new_register(uint64_t addr) {
    Register *res = (Register*) malloc( sizeof(Register) );
    if (res == NULL) 
        FATAL_DESC_ERROR("Cannot malloc() for new Register");

    res->addr   = addr;
    res->value  = 0x00;
    return res;
}

/*
 * function to add a register to the register map.
 *
 * resizes the array in the register_map,
 * adds the register to the array,
 * increments the reg_count
 */
static void add_register_to_map(Register *reg) {
    size_t newsize = sizeof(RegisterMap) + (register_map->reg_count + 1)*sizeof(Register*);
    register_map = (RegisterMap*) realloc(register_map, newsize);

    register_map->registers[ register_map->reg_count ] = reg;
    register_map->reg_count++;
}

/*
 * find a register in the registermap by it's address.
 */
static Register* find_register(uint64_t addr) {
    if( !register_exists(addr) )
        FATAL_DESC_ERROR("Cannot find register!");

    uint64_t i;
    for(i = 0 ; 
        i < register_map->reg_count && r == NULL && 
        register_map->registers[i]->addr != addr; 
        i++);
    return &register_map->registers[i];
}

/*
 * run function 
 *
 * runs the vm.  
 */
static void run() {
    while( !program_pointer_is( 0xFFFF ) ) {
        run_opcode(*((uint16_t*)minx_binary_get_at(program_pointer, OPC_SIZE)));
    }
}

static void run_opcode(uint16_t cmd) {
    void (*opc_func)(void) = opc_funcs[cmd];
    if( opc_func == NULL ) {
        FATAL_DESC_ERROR("Tried to execute unknown opcode!");
    }
    opc_func();
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
    opc_p->p1 = *((uint64_t *) minx_binary_get_at( ptr1_location, size1 ));
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

    opc_p->p1 = *((uint64_t *) minx_binary_get_at( ptr1_location, size1 ));
    opc_p->p2 = *((uint64_t *) minx_binary_get_at( ptr2_location, size2 ));
}

/*
 * Commands 
 * --------
 *
 * (Almost) each command has to modify the command pointer to the byte after 
 * the command.
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
 * If the command does affect the program pointer, this has not to be done!
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
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif
    program_pointer += (OPC_SIZE);
}

/*
 * Command:                 CALL
 * Parameters:              1
 * Affects Program Pointer: YES
 *
 * get current program_pointer, add enough for the next opcode, push it to stack,
 * set the program_pointer to the value of the argument of the opc.
 */
static void opc_call_func() {
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif
    read_1_command_parameter(ADDRESS_SIZE);
    stackpush(stack, program_pointer + OPC_SIZE + ADDRESS_SIZE );
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
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif
    if (stack_is_empty(stack))
        FATAL_DESC_ERROR("Cannot RET, stack is empty!");
     
    program_pointer = (uint64_t) stackpop();
}

/*
 * Command:                 MOV 
 * Parameters:              2: register-adress register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_mov_func() {
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif
    read_2_command_parameters(ADDRESS_SIZE, ADDRESS_SIZE);
    find_register(opc_p->p1)->value = find_register(opc_p->p2)->value;
    program_pointer += ( OPC_SIZE + ADDRESS_SIZE + ADDRESS_SIZE );
}

/*
 * Command:                 MOVI
 * Parameters:              2: register-address value
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_movi_func() {
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif

    read_2_command_parameters(ADDRESS_SIZE, VALUE_SIZE);
    find_register(opc_p->p1)->value = opc_p->p2;

    program_pointer += ( OPC_SIZE + ADDRESS_SIZE + VALUE_SIZE );
}

/*
 * Command:                 NOT 
 * Parameters:              1: register-address 
 * Affects Program Pointer: NO
 *
 * Result in akku
 */
static void opc_not_func() {
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif

    read_1_command_parameter(ADDRESS_SIZE);
    AKKU = ! find_register(opc_p->p1)->value;

    program_pointer += ( OPC_SIZE + ADDRESS_SIZE );
}

/*
 * Command:                 NOTR
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 * Result in same register
 */
static void opc_notr_func() {
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif

    read_1_command_parameter(ADDRESS_SIZE);
    find_register(opc_p->p1)->value = ! find_register(opc_p->p1)->value;
 
    program_pointer += ( OPC_SIZE + ADDRESS_SIZE );
}

/*
 * Command:                 AND
 * Parameters:              2: register-address register-address
 * Affects Program Pointer: NO
 *
 * result in akku
 */
static void opc_and_func() {
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif

    read_2_command_parameters(ADDRESS_SIZE, ADDRESS_SIZE);
    akku = find_register(opc_p->p1)->value & find_register(opc_p->p2)->value;

    program_pointer += ( OPC_SIZE + ADDRESS_SIZE + ADDRESS_SIZE );
}

/*
 * Command:                 ANDI
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 * Result in akku
 */
static void opc_andi_func() {
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif

    read_2_command_parameters(ADDRESS_SIZE, VALUE_SIZE);
    akku = find_register(opc_p->p1)->value & opc_p->p2;

    program_pointer += ( OPC_SIZE + ADDRESS_SIZE + ADDRESS_SIZE );
}

/*
 * Command:                 ANDR
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 * Result in first register
 */
static void opc_andr_func() {
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif

    read_2_command_parameters(ADDRESS_SIZE, ADDRESS_SIZE);
    find_register(opc_p->p1)->value &= find_register(opc_p->p2)->value;

    program_pointer += ( OPC_SIZE + ADDRESS_SIZE + ADDRESS_SIZE );
}

/*
 * Command:                 ANDIR
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 * Result in first register
 */
static void opc_andir_func() {
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif

    read_2_command_parameters(ADDRESS_SIZE, VALUE_SIZE);
    find_register(opc_p->p1)->value &= opc_p->p2;

    program_pointer += ( OPC_SIZE + ADDRESS_SIZE + VALUE_SIZE);
}


/*
 * Command:                 OR
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 * Result in akku
 */
static void opc_or_func() {
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif

    read_2_command_parameters(ADDRESS_SIZE, ADDRESS_SIZE);
    akku = find_register(opc_p->p1)->value | find_register(opc_p->p2)->value;

    program_pointer += ( OPC_SIZE + ADDRESS_SIZE + ADDRESS_SIZE );
}

/*
 * Command:                 ORI 
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_ori_func() {
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif

    read_2_command_parameters(ADDRESS_SIZE, VALUE_SIZE);
    akku = find_register(opc_p->p1)->value | opc_p->p2;

    program_pointer += ( OPC_SIZE + ADDRESS_SIZE + VALUE_SIZE);
}

/*
 * Command:                 ORR
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_orr_func() {
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif

    read_2_command_parameters(ADDRESS_SIZE, ADDRESS_SIZE);
    find_register(opc_p->p1)->value |= find_register(opc_p->p2)->value;

    program_pointer += ( OPC_SIZE + ADDRESS_SIZE + ADDRESS_SIZE);
}

/*
 * Command:                 ORIR
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_orir_func() {
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif

    read_2_command_parameters(ADDRESS_SIZE, VALUE_SIZE);
    find_register(opc_p->p1)->value |= opc_p->p2;

    program_pointer += ( OPC_SIZE + ADDRESS_SIZE + VALUE_SIZE);
}

/*
 * Command:                 DEC
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_dec_func() {
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif

    read_1_command_parameter(ADDRESS_SIZE);

    if( find_register(opc_p->p1)->value == 0x0000 ) {
        setbit(statusregister, OVERFLOW_BIT);
    }

    find_register(opc_p->p1)->value--;
    
    program_pointer += (OPC_SIZE + ADDRESS_SIZE);
}

/*
 * Command:                 INC
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_inc_func() {
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif
    read_1_command_parameter(ADDRESS_SIZE);

    if( find_register(opc_p->p1)->value == 0xFFFF ) {
        setbit(statusregister, OVERFLOW_BIT);
    }

    find_register(opc_p->p1)->value++;
    
    program_pointer += (OPC_SIZE + ADDRESS_SIZE);
}

/*
 * Command:                 LSH
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_lsh_func() {
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif
    read_1_command_parameter(ADDRESS_SIZE);
    find_register(opc_p->p1)->value<<1;
    
    program_pointer += (OPC_SIZE + ADDRESS_SIZE);
}

/*
 * Command:                 RSH
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_rsh_func() {
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif
    read_1_command_parameter(ADDRESS_SIZE);
    find_register(opc_p->p1)->value>>1;
    
    program_pointer += (OPC_SIZE + ADDRESS_SIZE);
}

/*
 * Command:                 PUSH
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_push_func() {
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif
    read_1_command_parameter(ADDRESS_SIZE);
    stackpush(stack, &(find_register(opc_p->p1)->value), VALUE_SIZE) ;
    
    program_pointer += (OPC_SIZE + ADDRESS_SIZE);
}

/*
 * Command:                 POP
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_pop_func() {
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif
    if( stack_is_empty( stack ) ) {
        FATAL_DESC_ERROR("Cannot pop from empty stack!");
    }

    read_1_command_parameter(ADDRESS_SIZE);
    find_register(opc_p->p1)->value = *((uint64_t*)stackpop(stack));
    
    program_pointer += (OPC_SIZE + ADDRESS_SIZE);
}

/*
 * Command:                 DROP
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_drop_func() {
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif
    if( stack_is_empty( stack ) ) {
        FATAL_DESC_ERROR("Cannot drop from empty stack!");
    }

    read_1_command_parameter(ADDRESS_SIZE);
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
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif
    read_2_command_parameters(ADDRESS_SIZE, ADDRESS_SIZE);
    akku = find_register(opc_p->p1)->value + find_register(opc_p->p2)->value;

    program_pointer += (OPC_SIZE + ADDRESS_SIZE + ADDRESS_SIZE);
}

/*
 * Command:                 ADDI
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 * Result in akku
 */
static void opc_addi_func() {
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif
    read_2_command_parameters(ADDRESS_SIZE, VALUE_SIZE);
    akku = find_register(opc_p->p1)->value + opc_p->p2;

    program_pointer += (OPC_SIZE + ADDRESS_SIZE + VALUE_SIZE);
}

/*
 * Command:                 ADDR
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 * Result in register
 */
static void opc_addr_func() {
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif
    read_2_command_parameters(ADDRESS_SIZE, ADDRESS_SIZE);
    find_register(opc_p->p1)->value += find_register(opc_p->p2)->value;

    program_pointer += (OPC_SIZE + ADDRESS_SIZE + ADDRESS_SIZE);
}

/*
 * Command:                 ADDIR
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 * Result in register
 */
static void opc_addir_func() {
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif
    read_2_command_parameters(ADDRESS_SIZE, VALUE_SIZE);
    find_register(opc_p->p1)->value += opc_p->p2;

    program_pointer += (OPC_SIZE + ADDRESS_SIZE + VALUE_SIZE);
}

/*
 * Command:                 JMP
 * Parameters:              1: address
 * Affects Program Pointer: YES
 *
 *
 */
static void opc_jmp_func() {
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif
    read_1_command_parameter(ADDRESS_SIZE);
    if( minx_binary_exists_at(opc_p->p1) ) {
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
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif
    read_2_command_parameters(ADDRESS_SIZE, ADDRESS_SIZE);
    if( minx_binary_exists_at(opc_p->p2) ) {
        if( find_register(opc_p->p1)->value == 0x0000 ) {
            program_pointer = opc_p->p2;
        }
        else {
            program_pointer += ( OPC_SIZE + ADDRESS_SIZE + ADDRESS_SIZE );
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
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif
    read_2_command_parameters(ADDRESS_SIZE, ADDRESS_SIZE);
    if( minx_binary_exists_at(opc_p->p2) ) {
        if( find_register(opc_p->p1)->value != 0x0000 ) {
            program_pointer = opc_p->p2;
        }
        else {
            program_pointer += ( OPC_SIZE + ADDRESS_SIZE + ADDRESS_SIZE );
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
#ifdef DEBUG
    EXPLAIN_OPCODE();
#endif
    read_2_command_parameters(ADDRESS_SIZE, ADDRESS_SIZE);
    if(minx_binary_exists_at(opc_p->p1) && minx_binary_exists_at(opc_p->p2)) {
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
 * "static" macros undef  
 * ---------------------
 */ 
#undef program_pointer
#undef akku
#undef statusregister

#undef register_exists
#undef program_pointer_is
