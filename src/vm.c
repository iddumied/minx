#include "vm.h"

/*
 * static variables
 */

static Register *   registers       = NULL;
static unsigned int registers_cnt   = 0;

static Stack    *   stack           = NULL;

/*
 * static functions prototypes
 */

static void         alloc_standard_registers    (void);
static void         new_register                (void);
static void         shutdown                    (void);
static void         run                         (void);

/*
 * Commands
 *
 */

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
    alloc_standard_registers();
    init_stack();
    run();

    shutdown();
}

/* static functions */

static void alloc_standard_registers() {
    for( registers_cnt = 0 ; registers_cnt < 8 ; registers_cnt++ ) {
        new_register();
    }
}

static void init_stack() {
    stack = empty_stack();
}

static void new_register() {
    if( registers == NULL )
        registers = (Register*) malloc( sizeof(*registers) );
    else 
        registers = (Register*) realloc( registers, sizeof(Register) * (registers_cnt+1) );

#define curr_r (&(registers[registers_cnt]))
    curr_r->value = (uint64_t*) malloc( sizeof(curr_r->value) );

    if( curr_r->value == NULL ) 
        FATAL_DESC_ERROR("Could not malloc() space for register");

    memset( &curr_r->value, 0x0, sizeof(curr_r->value));
#undef curr_r

    registers_cnt++;
    
}

static void shutdown() {
    free(registers);
    stackdelete(stack);
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

        case C_STARTAT: 
            command_startat();
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
static CommandParameters* read_1_command_parameter(CommandParameters *cp,
                                                    unsigned int size1) {
    uint64_t ptr1_location = program_pointer + COMMAND_SIZE;
    cp->p1 = minx_binary_get_at( ptr1_location, size1 );
    
    return cp;
}

/*
 * Helper function for parsing command parameters
 * args:
 *
 * @param cp            storage for the result 
 * @param size1         size of argument 1
 * @param offset2       offset of the program_pointer to the value of arg 2
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
static CommandParameters* read_2_command_parameters(CommandParameters *cp,
                                                    unsigned int size1,
                                                    unsigned int offset2,
                                                    unsigned int size2) {

    uint64_t ptr1_location = program_pointer + COMMAND_SIZE;
    uint64_t ptr2_location = program_pointer + COMMAND_SIZE + offset2;

    cp->p1 = minx_binary_get_at( ptr1_location, size1 );
    cp->p2 = minx_binary_get_at( ptr2_location, size2 );
    
    return cp;
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
}

/*
 * Command:                 MOV 
 * Parameters:              2: register-adress register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void command_mov() {
}

/*
 * Command:                 MOVI
 * Parameters:              2: register-address value
 * Affects Program Pointer: NO
 *
 *
 */
static void command_movi() {
}

/*
 * Command:                 NOT 
 * Parameters:              1: register-address 
 * Affects Program Pointer: NO
 *
 * Result in akku
 */
static void command_not() {
}

/*
 * Command:                 NOTR
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 * Result in same register
 */
static void command_notr() {
}

/*
 * Command:                 AND
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 * result in akku
 */
static void command_and() {
}

/*
 * Command:                 ANDI
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 * Result in akku
 */
static void command_andi() {
}

/*
 * Command:                 ANDR
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 * Result in first register
 */
static void command_andr() {
}

/*
 * Command:                 ANDIR
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 * Result in first register
 */
static void command_andir() {
}


/*
 * Command:                 OR
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 * Result in akku
 */
static void command_or() {
}

/*
 * Command: 
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 *
 */
static void command_ori() {
}

/*
 * Command: 
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 *
 */
static void command_orr() {
}

/*
 * Command: 
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 *
 */
static void command_orir() {
}

/*
 * Command: 
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 *
 */
static void command_dec() {
}

/*
 * Command: 
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 *
 */
static void command_inc() {
}

/*
 * Command: 
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 *
 */
static void command_lsh() {
}

/*
 * Command: 
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 *
 */
static void command_rsh() {
}

/*
 * Command: 
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 *
 */
static void command_push() {
}

/*
 * Command: 
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 *
 */
static void command_pop() {
}

/*
 * Command: 
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 *
 */
static void command_drop() {
}

/*
 * Command: 
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 *
 */
static void command_add() {
}

/*
 * Command: 
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 *
 */
static void command_addi() {
}

/*
 * Command: 
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 *
 */
static void command_addr() {
}

/*
 * Command: 
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 *
 */
static void command_addir() {
}

/*
 * Command: 
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 *
 */
static void command_jmp() {
}

/*
 * Command: 
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 *
 */
static void command_jmpiz() {
}

/*
 * Command: 
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 *
 */
static void command_jmpnz() {
}

/*
 * Command: 
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 *
 */
static void command_ifzjmp() {
}

/*
 * Command: 
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 *
 */
static void command_startat() {
}

/*
 * Helper functions
 */

static void inc_program_pointer(uint64_t number_of_bytes) {
    registers[0].value += number_of_bytes;
}
