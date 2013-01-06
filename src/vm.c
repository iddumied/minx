#include "vm.h"

/*
 * static variables
 */

static Register             *   registers               = NULL;
static unsigned int             registers_cnt           = 0;

static Stack                *   stack                   = NULL;
static CommandParameters    *   cmd_p                   = NULL;

/*
 * static functions prototypes
 */

static void         new_register                (void);
static void         run                         (void);

/*
 * Commands
 * --------
 */

static void read_1_command_parameter(unsigned int size1);
static void read_2_command_parameters(unsigned int size1, unsigned int size2);

// static void command_nop();
static void         command_return(void);

static void         command_mov(void);
static void         command_movi(void);

static void         command_not(void);
static void         command_notr(void);

static void         command_and(void);
static void         command_andi(void);
static void         command_andr(void);
static void         command_andir(void);

static void         command_or(void);
static void         command_ori(void);
static void         command_orr(void);
static void         command_orir(void);

static void         command_dec(void);
static void         command_inc(void);

static void         command_lsh(void);
static void         command_rsh(void);

static void         command_push(void);
static void         command_pop(void);
static void         command_drop(void);

static void         command_add(void);
static void         command_addi(void);
static void         command_addr(void);
static void         command_addir(void);

static void         command_jmp(void);
static void         command_jmpiz(void);
static void         command_jmpnz(void);
static void         command_ifzjmp(void);

static void         command_startat(void);

/* NOT SUPPORTED YET
static void         command_lnreg();
static void         command_lpreg();

static void         command_getp();
static void         command_getpa();
*/

/*
 * Functions
 */

void minx_vm_run() {
    /* alloc standard registers */
    for( registers_cnt = 0x0000 ; registers_cnt < 0x000F ; registers_cnt++ ) {
        new_register();
    }

    stack = empty_stack();
    cmd_p = (CommandParameters*) malloc( sizeof(*cmd_p) );

    run();

    free(registers);
    free(cmd_p);
    stackdelete(stack);
}

/* static functions */

static void new_register() {
    if( registers == NULL ) {
        registers = (Register*) malloc( sizeof(*registers) );
    }
    else {
        registers = (Register*) realloc( registers, sizeof(Register) * (registers_cnt+1) );
    }

#define curr_r (&(registers[registers_cnt]))
    curr_r->value = (uint64_t*) malloc( sizeof(curr_r->value) );

    if( curr_r->value == NULL ) 
        FATAL_DESC_ERROR("Could not malloc() space for register");

    memset( &curr_r->value, 0x0, sizeof(curr_r->value));
#undef curr_r

    registers_cnt++;
}

/*
 * execution
 */

static void run() {

    uint64_t cmd_p;
    uint16_t cmd;

    while( !program_pointer_is( 0xFFFF ) ) {
        cmd_p  = get_current_command_ptr();
        cmd    = minx_binary_get_at(cmd_p, COMMAND_SIZE); 
        run_command(cmd);
    }

}

static signed int program_pointer_is( uint64_t val ) {
    return registers[0].value == val;
}

static uint64_t get_current_command_ptr() {
    return registers[0].value;
}

static void run_command(uint16_t cmd) {
    switch( cmd ) {

        case C_RET: 
            command_return();
            break;

        case C_MOV: 
            command_mov();
            break;

        case C_MOVI: 
            command_movi();
            break;

        case C_NOT: 
            command_not();
            break;

        case C_NOTR: 
            command_notr();
            break;

        case C_AND: 
            command_and();
            break;

        case C_ANDI: 
            command_andi();
            break;

        case C_ANDR: 
            command_andr();
            break;

        case C_ANDIR: 
            command_andir();
            break;

        case C_OR: 
            command_or();
            break;

        case C_ORI: 
            command_ori();
            break;

        case C_ORR: 
            command_orr();
            break;

        case C_ORIR: 
            command_orir();
            break;

        case C_DEC: 
            command_dec();
            break;

        case C_INC: 
            command_inc();
            break;

        case C_LSH: 
            command_lsh();
            break;

        case C_RSH: 
            command_rsh();
            break;

        case C_PUSH: 
            command_push();
            break;

        case C_POP: 
            command_pop();
            break;

        case C_DROP: 
            command_drop();
            break;

        case C_ADD: 
            command_add();
            break;

        case C_ADDI: 
            command_addi();
            break;

        case C_ADDR: 
            command_addr();
            break;

        case C_ADDIR: 
            command_addir();
            break;

        case C_JMP: 
            command_jmp();
            break;

        case C_JMPIZ: 
            command_jmpiz();
            break;

        case C_JMPNZ: 
            command_jmpnz();
            break;

        case C_IFZJMP: 
            command_ifzjmp();
            break;

    }

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
    uint64_t ptr1_location = program_pointer + COMMAND_SIZE;
    cmd_p->p1 = minx_binary_get_at( ptr1_location, size1 );
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
 *      - go to (program_pointer + COMMAND_SIZE )
 *      - read the value there, it could be a constant or a pointer, doesn't
 *      matter yet
 *      - go to (program_pointer + COMMAND_SIZE + <size of prev read data>)
 *      - read the value there, it could be a constant or a pointer, doesn't
 *      matter yet
 *
 *  and then use this data. The upper part is done by this helper function, to
 *  simplify the work of the command_ functions.
 */
static void read_2_command_parameters(unsigned int size1, unsigned int size2) {

    uint64_t ptr1_location = program_pointer + COMMAND_SIZE;
    uint64_t ptr2_location = program_pointer + COMMAND_SIZE + size1;

    cmd_p->p1 = minx_binary_get_at( ptr1_location, size1 );
    cmd_p->p2 = minx_binary_get_at( ptr2_location, size2 );
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
 * Command:                 RET
 * Parameters:              0
 * Affects Program Pointer: YES
 *
 *
 */
static void command_return() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif

    /*
     * if (stack_is_empty())
     *      FATAL_DESC_ERROR("Cannot RET, stack is empty!");
     */
    program_pointer = (uint64_t) stackpop();
}

/*
 * Command:                 MOV 
 * Parameters:              2: register-adress register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void command_mov() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif

    read_2_command_parameters(ADDRESS_SIZE, ADDRESS_SIZE);
    registers[cmd_p->p1].value = registers[cmd_p->p2];

    inc_program_pointer( COMMAND_SIZE + ADDRESS_SIZE + ADDRESS_SIZE );
}

/*
 * Command:                 MOVI
 * Parameters:              2: register-address value
 * Affects Program Pointer: NO
 *
 *
 */
static void command_movi() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif

    read_2_command_parameters(ADDRESS_SIZE, VALUE_SIZE);
    registers[cmd_p->p1].value  =   cmd_p->p2;

    inc_program_pointer( COMMAND_SIZE + ADDRESS_SIZE + VALUE_SIZE );
}

/*
 * Command:                 NOT 
 * Parameters:              1: register-address 
 * Affects Program Pointer: NO
 *
 * Result in akku
 */
static void command_not() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif

    read_1_command_parameter(ADDRESS_SIZE);
    akku = ! registers[cmd_p->p1].value;

    inc_program_pointer( COMMAND_SIZE + ADDRESS_SIZE );
}

/*
 * Command:                 NOTR
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 * Result in same register
 */
static void command_notr() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif

    read_1_command_parameter(ADDRESS_SIZE);
    registers[cmd_p->p1].value = ! registers[cmd_p->p1].value;
 
    inc_program_pointer( COMMAND_SIZE + ADDRESS_SIZE );
}

/*
 * Command:                 AND
 * Parameters:              2: register-address register-address
 * Affects Program Pointer: NO
 *
 * result in akku
 */
static void command_and() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif

    read_2_command_parameters(ADDRESS_SIZE, ADDRESS_SIZE);
    akku = registers[cmd_p->p1].value & registers[cmd_p->p2].value;

    inc_program_pointer( COMMAND_SIZE + ADDRESS_SIZE + ADDRESS_SIZE );
}

/*
 * Command:                 ANDI
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 * Result in akku
 */
static void command_andi() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif

    read_2_command_parameters(ADDRESS_SIZE, VALUE_SIZE);
    akku = registers[cmd_p->p1].value & cmd_p->p2;

    inc_program_pointer( COMMAND_SIZE + ADDRESS_SIZE + ADDRESS_SIZE );
}

/*
 * Command:                 ANDR
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 * Result in first register
 */
static void command_andr() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif

    read_2_command_parameters(ADDRESS_SIZE, ADDRESS_SIZE);
    registers[cmd_p->p1].value &= registers[cmd_p->p2].value;

    inc_program_pointer( COMMAND_SIZE + ADDRESS_SIZE + ADDRESS_SIZE );
}

/*
 * Command:                 ANDIR
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 * Result in first register
 */
static void command_andir() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif

    read_2_command_parameters(ADDRESS_SIZE, VALUE_SIZE);
    registers[cmd_p->p1].value &= cmd_p->p2;

    inc_program_pointer( COMMAND_SIZE + ADDRESS_SIZE + VALUE_SIZE);
}


/*
 * Command:                 OR
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 * Result in akku
 */
static void command_or() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif

    read_2_command_parameters(ADDRESS_SIZE, ADDRESS_SIZE);
    akku = registers[cmd_p->p1].value | registers[cmd_p->p2].value;

    inc_program_pointer( COMMAND_SIZE + ADDRESS_SIZE + ADDRESS_SIZE );
}

/*
 * Command:                 ORI 
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 *
 */
static void command_ori() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif

    read_2_command_parameters(ADDRESS_SIZE, VALUE_SIZE);
    akku = registers[cmd_p->p1].value | cmd_p->p2;

    inc_program_pointer( COMMAND_SIZE + ADDRESS_SIZE + VALUE_SIZE);
}

/*
 * Command:                 ORR
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void command_orr() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif

    read_2_command_parameters(ADDRESS_SIZE, ADDRESS_SIZE);
    registers[cmd_p->p1].value |= registers[cmd_p->p2].value;

    inc_program_pointer( COMMAND_SIZE + ADDRESS_SIZE + ADDRESS_SIZE);
}

/*
 * Command:                 ORIR
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 *
 */
static void command_orir() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif

    read_2_command_parameters(ADDRESS_SIZE, VALUE_SIZE);
    registers[cmd_p->p1].value |= cmd_p->p2;

    inc_program_pointer( COMMAND_SIZE + ADDRESS_SIZE + VALUE_SIZE);
}

/*
 * Command:                 DEC
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void command_dec() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif

    read_1_command_parameter(ADDRESS_SIZE);

    if( registers[cmd_p->p1].value == 0x0000 ) {
        setbit(statusregister, OVERFLOW_BIT);
    }

    registers[cmd_p->p1].value--;
    
    inc_program_pointer(COMMAND_SIZE + ADDRESS_SIZE);
}

/*
 * Command:                 INC
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void command_inc() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif
    read_1_command_parameter(ADDRESS_SIZE);

    if( registers[cmd_p->p1].value == 0xFFFF ) {
        setbit(statusregister, OVERFLOW_BIT);
    }

    registers[cmd_p->p1].value++;
    
    inc_program_pointer(COMMAND_SIZE + ADDRESS_SIZE);
}

/*
 * Command:                 LSH
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void command_lsh() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif
    read_1_command_parameter(ADDRESS_SIZE);
    registers[cmd_p->p1].value<<1;
    
    inc_program_pointer(COMMAND_SIZE + ADDRESS_SIZE);
}

/*
 * Command:                 RSH
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void command_rsh() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif
    read_1_command_parameter(ADDRESS_SIZE);
    registers[cmd_p->p1].value>>1;
    
    inc_program_pointer(COMMAND_SIZE + ADDRESS_SIZE);
}

/*
 * Command:                 PUSH
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void command_push() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif
    read_1_command_parameter(ADDRESS_SIZE);
    stackpush(stack, &(registers[cmd_p->p1].value), VALUE_SIZE) ;
    
    inc_program_pointer(COMMAND_SIZE + ADDRESS_SIZE);
}

/*
 * Command:                 POP
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void command_pop() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif
    if( stack_is_empty( stack ) ) {
        FATAL_DESC_ERROR("Cannot pop from empty stack!");
    }

    read_1_command_parameter(ADDRESS_SIZE);
    registers[cmd_p->p1].value = *((uint64_t*)stackpop(stack));
    
    inc_program_pointer(COMMAND_SIZE + ADDRESS_SIZE);
}

/*
 * Command:                 DROP
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 *
 */
static void command_drop() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif
    if( stack_is_empty( stack ) ) {
        FATAL_DESC_ERROR("Cannot drop from empty stack!");
    }

    read_1_command_parameter(ADDRESS_SIZE);
    stackpop(stack);
    
    inc_program_pointer(COMMAND_SIZE);
}

/*
 * Command:                 ADD
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 * Result in akku
 */
static void command_add() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif
    read_2_command_parameters(ADDRESS_SIZE, ADDRESS_SIZE);
    akku = registers[cmd_p->p1].value + registers[cmd_p->p2].value;

    inc_program_pointer(COMMAND_SIZE + ADDRESS_SIZE + ADDRESS_SIZE);
}

/*
 * Command:                 ADDI
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 * Result in akku
 */
static void command_addi() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif
    read_2_command_parameters(ADDRESS_SIZE, VALUE_SIZE);
    akku = registers[cmd_p->p1].value + cmd_p->p2;

    inc_program_pointer(COMMAND_SIZE + ADDRESS_SIZE + VALUE_SIZE);
}

/*
 * Command:                 ADDR
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 * Result in register
 */
static void command_addr() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif
    read_2_command_parameters(ADDRESS_SIZE, ADDRESS_SIZE);
    registers[cmd_p->p1].value += registers[cmd_p->p2].value;

    inc_program_pointer(COMMAND_SIZE + ADDRESS_SIZE + ADDRESS_SIZE);
}

/*
 * Command:                 ADDIR
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 * Result in register
 */
static void command_addir() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif
    read_2_command_parameters(ADDRESS_SIZE, VALUE_SIZE);
    registers[cmd_p->p1].value += cmd_p->p2;

    inc_program_pointer(COMMAND_SIZE + ADDRESS_SIZE + VALUE_SIZE);
}

/*
 * Command:                 JMP
 * Parameters:              1: address
 * Affects Program Pointer: YES
 *
 *
 */
static void command_jmp() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif
    read_1_command_parameter(ADDRESS_SIZE);
    if( minx_binary_exists_at(cmd_p->p1) ) {
        program_pointer = cmd_p->p1;
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
static void command_jmpiz() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif
    read_2_command_parameters(ADDRESS_SIZE, ADDRESS_SIZE);
    if( minx_binary_exists_at(cmp_p->p2) ) {
        if( registers[cmd_p->p1].value == 0x0000 ) {
            program_pointer = cmd_p->p2;
        }
        else {
            inc_program_pointer( COMMAND_SIZE + ADDRESS_SIZE + ADDRESS_SIZE );
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
static void command_jmpnz() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif
    read_2_command_parameters(ADDRESS_SIZE, ADDRESS_SIZE);
    if( minx_binary_exists_at(cmp_p->p2) ) {
        if( registers[cmd_p->p1].value != 0x0000 ) {
            program_pointer = cmd_p->p2;
        }
        else {
            inc_program_pointer( COMMAND_SIZE + ADDRESS_SIZE + ADDRESS_SIZE );
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
static void command_ifzjmp() {
#ifdef DEBUG
    EXPLAIN_COMMAND();
#endif
    read_2_command_parameters(ADDRESS_SIZE, ADDRESS_SIZE);
    if(minx_binary_exists_at(cmp_p->p1) && minx_binary_exists_at(cmd_p->p2)) {
        if( akku == 0x0000 ) {
            program_pointer = cmd_p->p1;
        }
        else {
            program_pointer = cmd_p->p2;
        }
    } 
    else {
        FATAL_DESC_ERROR("Cannot jump");
    }
}

/*
 * Helper functions
 */

static void inc_program_pointer(uint64_t number_of_bytes) {
    registers[0].value += number_of_bytes;
}
