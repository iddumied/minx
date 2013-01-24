#include "vpu.h"

/*
 * static functions prototypes
 * ---------------------------
 */

static void         init_registers                  (void);
static Register*    find_register                   (uint64_t addr);

static void         run                             (void);

static void         read_n_command_parameters       (unsigned int n,
                                                    unsigned int sizes[]);

static void         run_opcode                      (uint16_t);

static void         setup_heap                      (void);
static void         shutdown_heap                   (void);
static HeapNode*    find_heapnode                   (uint64_t ptr);
static HeapNode*    find_or_create_unused_heapnode  (void);
static char*        get_memory_from_heapnode        (HeapNode *h, uint64_t addr);

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
static void         opc_pmems_func          (void);
static void         opc_pmem_func           (void);

static void         opc_alloc_func          (void);
static void         opc_alloci_func         (void);
static void         opc_resize_func         (void);
static void         opc_resizei_func        (void);
static void         opc_free_func           (void);
static void         opc_put_func            (void);
static void         opc_read_func           (void);
static void         opc_getsize_func        (void);

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
static HeapNode             *   heapnodes       = NULL;
static uint64_t                 heapnodes_count = 0;

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
    [OPC_PPROG]     = opc_pprog_func,
    [OPC_PMEMS]     = opc_pmems_func,
    [OPC_PMEM]      = opc_pmem_func,

    [OPC_ALLOC]     = opc_alloc_func,
    [OPC_ALLOCI]    = opc_alloci_func,
    [OPC_RESIZE]    = opc_resize_func,
    [OPC_RESIZEI]   = opc_resizei_func,
    [OPC_FREE]      = opc_free_func,
    [OPC_PUT]       = opc_put_func,
    [OPC_READ]      = opc_read_func,
    [OPC_GETSIZE]   = opc_getsize_func,
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

void minx_vpu_run() {
#if (defined DEBUGGING | defined DEBUG)
    minxvpudbgprint("Starting\n");
#endif

    /* alloc standard registers */
    init_registers();
    setup_heap();
    stack = empty_stack();
    opc_p = (CommandParameters*) malloc( sizeof(*opc_p) );

    run();

    free(registers);
    free(opc_p);
    stackdelete(stack);
    shutdown_heap();
}

/* 
 * static functions 
 * ----------------
 */

static void init_registers() {
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
    minxvpudbgprintf("find register: %"PRIu64"\n", addr);
#endif 

    if( !register_exists(addr) ) {
        FATAL_DESC_ERROR("Register does not exist");
    }
    return &registers[ addr & REGISTER_MASK ]; 
}

/*
 * run function 
 *
 * runs the vpu.  
 */
static void run() {
#if (defined DEBUGGING | defined DEBUG)
    minxvpudbgprint("run");
#endif //DEBUGGING

    uint16_t *opcode = (uint16_t*) malloc(sizeof(*opcode));

    while( !program_pointer_is(END_OF_PROGRAM) ) {

#if (defined DEBUGGING || defined DEBUG)
        fflush(stdout);
#endif // (defined DEBUGGING || defined DEBUG)
        opcode = minx_binary_get_at(program_pointer, OPC_SIZE, opcode);
        run_opcode(*opcode);
    }

#if (defined VERBOSITY)
    if( minx_config_get(CONF_PRINT_REGS_AT_EOP)->b ) {
        unsigned int i;
        for( i = 0; i < register_count ; i++ ) {
            print_register(i);
        }
    }
#endif //if (defined VERBOSITY)

    free(opcode);
} //static void run()

/*
 * runs the opcode, passed to the function by finding it in the opc_funcs array.
 *
 * @param cmd the opcode to run.
 *
 */
static void run_opcode(uint16_t cmd) {
#if (defined DEBUGGING | defined DEBUG)
    minxvpudbgprintf("Running opcode: %"PRIu16"\n", cmd);
    fflush(stdout);
#endif 

    void (*opc_func)(void) = opc_funcs[cmd];
    if( opc_func == NULL ) {
        FATAL_DESC_ERROR("Tried to execute unknown opcode!");
    }
    opc_func();

#if (defined DEBUGGING | defined DEBUG)
    if ( program_pointer != END_OF_PROGRAM ) {
        minxvpudbgprintf("[minx][VPU]:\tPROG_POINTER: %"PRIu64"\n", program_pointer);
    }
    else {
        minxvpudbgprint("[minx][VPU]: END OF PROGRAM\n");
    }
#endif 
}

/*
 * -----------------------------------------------------------------------------
 *
 *      Heap section - all helper functions for heap management
 *
 * -----------------------------------------------------------------------------
 */

static void setup_heap() {
    heapnodes = (HeapNode*) malloc( sizeof(HeapNode) );
    heapnodes[0].used               = HEAPNODE_NOT_USED;
    heapnodes[0].first_byte_addr    = (uint64_t) 0x00;
    heapnodes[0].size               = (uint64_t) 0x00;
    heapnodes[0].memory             = NULL;
    heapnodes_count = 1;
}

static void shutdown_heap() {
    uint64_t i;
    for(i = 0 ; i < heapnodes_count ; i++) {
        free(heapnodes[i].memory);
    }
    free(heapnodes);
}

/*
 * find heapnode by address, if not found, return NULL.
 *
 * TODO: 
 * If the address is INSIDE of a heapnode, the heapnode has to be returned!
 */
static HeapNode* find_heapnode(uint64_t ptr) {
    uint64_t i;
    for(i = 0; i < heapnodes_count && heapnodes[i].first_byte_addr != ptr; i++);
    if( i == heapnodes_count ) {
        return NULL;
    }
    return &(heapnodes[i]);
}

/*
 * finds a unused heapnode in the heapnodes-array or creates a new one.
 */
static HeapNode* find_or_create_unused_heapnode() {
    HeapNode *new = NULL;
    uint64_t i;
    for( i = 0 ; i < heapnodes_count ; i++ ) {
        if( heapnodes[i].used == HEAPNODE_NOT_USED ) {
            new = &heapnodes[i];
            break;
        }
    }
    if( new == NULL ) {

        /*
         * reallocate with more heapnodes than required for future use.
         */
        unsigned int new_heapnodes_count;

        if( heapnodes_count < 20 ) {
            new_heapnodes_count = 10;
        }
        else if ( heapnodes_count < 50 ) {
            new_heapnodes_count = 5;
        }
        else {
            new_heapnodes_count = 2;
        }

        heapnodes = realloc(heapnodes, sizeof(HeapNode) * (heapnodes_count+new_heapnodes_count));

        for( i = (heapnodes_count + new_heapnodes_count -1 ); i > heapnodes_count; i--) {
            heapnodes[i].used = HEAPNODE_NOT_USED;
            heapnodes[i].first_byte_addr    = (uint64_t) 0x00;
            heapnodes[i].size               = (uint64_t) 0x00;
            heapnodes[i].memory             = NULL;
        }
        new = &heapnodes[heapnodes_count];
        heapnodes_count += new_heapnodes_count;
    }

    /*
     * calculate memorysize for the new heapnode
     */

    uint64_t ptr = 0;
    for( i = 0 ; &heapnodes[i] != new; i++,  ptr += heapnodes[i].size );
    new->first_byte_addr = ptr;
    return new;
}

static char* get_memory_from_heapnode(HeapNode *h, uint64_t addr) {
    char * mem = (char*)h->memory;
    if( h->first_byte_addr > addr || h->first_byte_addr - addr > h->size ) {
        FATAL_DESC_ERROR("tried to access memory that does not exist");
    }
    uint64_t local_ptr = h->first_byte_addr - addr;
    return &mem[local_ptr];
}


/*
 * -----------------------------------------------------------------------------
 *
 *      Argument section - all helpers for opcode arguments 
 *
 * -----------------------------------------------------------------------------
 */

/*
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
 */
static void read_n_command_parameters(unsigned int n, unsigned int sizes[]) {
    uint64_t        next_pos = program_pointer + OPC_SIZE;
    unsigned int    i;

    for(i = 0 ; i < n ; i++ ) {
        opc_p->p[i] = *((uint64_t*) minx_binary_get_at(next_pos, sizes[i], &opc_p->p[i]));
        next_pos += sizes[i];
    }
}

/*
 * -----------------------------------------------------------------------------
 *
 *      Opcodes functions
 *
 * -----------------------------------------------------------------------------
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
    unsigned int params[] = { PROGRAM_ADDRESS_SIZE };
    read_n_command_parameters(1, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("call", "%"PRIu64, opc_p->p[1]);
#endif
    /*
     * push to stack
     *
     * push the address of the next opcode to the stack,
     * the address has size (PROGRAM_ADDRESS_SIZE) (in this case this is 8 Byte)
     */
    stackpush(  stack,
                (void*)(program_pointer + OPC_SIZE + PROGRAM_ADDRESS_SIZE), 
                (size_t)PROGRAM_ADDRESS_SIZE);
    program_pointer = opc_p->p[0];
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
     
    program_pointer = *((uint64_t*) stackpop(stack));
}

/*
 * Command:                 MOV 
 * Parameters:              2: register-adress register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_mov_func() {
    unsigned int params[] = { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE };
    read_n_command_parameters(2, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("mov", "%"PRIu64" <- %"PRIu64, opc_p->p[1], opc_p->p[2]);
#endif 

    find_register(opc_p->p[1])->value = find_register(opc_p->p[2])->value;
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
    unsigned int params[] = { REGISTER_ADDRESS_SIZE, VALUE_SIZE };
    read_n_command_parameters(2, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("movi", "%"PRIu64" <- %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif 

    find_register(opc_p->p[0])->value = opc_p->p[1];

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
    unsigned int params[] = { REGISTER_ADDRESS_SIZE };
    read_n_command_parameters(1, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("not", "reg: %"PRIu64, opc_p->p[1]);
#endif 

    akku = ! find_register(opc_p->p[1])->value;

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
    unsigned int params[] = { REGISTER_ADDRESS_SIZE };
    read_n_command_parameters(1, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("not", "reg: %"PRIu64, opc_p->p[0]);
#endif 

    find_register(opc_p->p[0])->value = ! find_register(opc_p->p[0])->value;
 
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
    unsigned int params[] = { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE };
    read_n_command_parameters(2, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("and", "reg %"PRIu64" & reg %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif

    akku = find_register(opc_p->p[1])->value & find_register(opc_p->p[1])->value;

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
    unsigned int params[] = { REGISTER_ADDRESS_SIZE, VALUE_SIZE };
    read_n_command_parameters(2, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("andi", "reg %"PRIu64" & %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif

    akku = find_register(opc_p->p[0])->value & opc_p->p[1];

    program_pointer += ( OPC_SIZE + REGISTER_ADDRESS_SIZE + VALUE_SIZE );
}

/*
 * Command:                 ANDR
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 * Result in first register
 */
static void opc_andr_func() {
    unsigned int params[] = { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE };
    read_n_command_parameters(2, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("andr", "reg %"PRIu64" & reg%"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif

    find_register(opc_p->p[0])->value &= find_register(opc_p->p[1])->value;

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
    unsigned int params[] = { REGISTER_ADDRESS_SIZE, VALUE_SIZE };
    read_n_command_parameters(2, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("andir", "reg %"PRIu64" & %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif

    find_register(opc_p->p[0])->value &= opc_p->p[1];

    program_pointer += ( OPC_SIZE + REGISTER_ADDRESS_SIZE + VALUE_SIZE );
}


/*
 * Command:                 OR
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 * Result in akku
 */
static void opc_or_func() {
    unsigned int params[] = { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE };
    read_n_command_parameters(2, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("or", "reg %"PRIu64" | reg %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif 

    akku = find_register(opc_p->p[0])->value | find_register(opc_p->p[1])->value;

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
    unsigned int params[] = { REGISTER_ADDRESS_SIZE, VALUE_SIZE };
    read_n_command_parameters(2, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("ori", "reg %"PRIu64" | %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif 

    akku = find_register(opc_p->p[0])->value | opc_p->p[1];

    program_pointer += ( OPC_SIZE + REGISTER_ADDRESS_SIZE + VALUE_SIZE );
}

/*
 * Command:                 ORR
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_orr_func() {
    unsigned int params[] = { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE };
    read_n_command_parameters(2, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("orr", "reg %"PRIu64" | reg %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif 

    find_register(opc_p->p[0])->value |= find_register(opc_p->p[1])->value;

    program_pointer += ( OPC_SIZE + REGISTER_ADDRESS_SIZE + REGISTER_ADDRESS_SIZE );
}

/*
 * Command:                 ORIR
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_orir_func() {
    unsigned int params[] = { REGISTER_ADDRESS_SIZE, VALUE_SIZE };
    read_n_command_parameters(2, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("orir", "reg %"PRIu64" | %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif 

    find_register(opc_p->p[0])->value |= opc_p->p[1];

    program_pointer += ( OPC_SIZE + REGISTER_ADDRESS_SIZE + VALUE_SIZE );
}

/*
 * Command:                 DEC
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_dec_func() {
    unsigned int params[] = { REGISTER_ADDRESS_SIZE };
    read_n_command_parameters(1, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("dec", "reg: %"PRIu64, opc_p->p[0]);
#endif 

    if( find_register(opc_p->p[0])->value == 0x0000 ) {
        setbit(statusregister, OVERFLOW_BIT);
    }

    find_register(opc_p->p[0])->value--;
    
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
    unsigned int params[] = { REGISTER_ADDRESS_SIZE };
    read_n_command_parameters(1, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("inc", "reg: %"PRIu64, opc_p->p[0]);
#endif 

    if( find_register(opc_p->p[0])->value == 0xFFFF ) {
        setbit(statusregister, OVERFLOW_BIT);
    }

    find_register(opc_p->p[0])->value++;
    
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
    unsigned int params[] = { REGISTER_ADDRESS_SIZE };
    read_n_command_parameters(1, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("lsh", "reg %"PRIu64, opc_p->p[0]);
#endif

    Register *r = find_register(opc_p->p[0]);
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
    unsigned int params[] = { REGISTER_ADDRESS_SIZE };
    read_n_command_parameters(1, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("rsh", "reg %"PRIu64, opc_p->p[0]);
#endif

    Register *r = find_register(opc_p->p[0]);
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
    unsigned int params[] = { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE };
    read_n_command_parameters(2, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("cmp", "reg %"PRIu64" and reg %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif 
    
    Register * r1 = find_register(opc_p->p[0]);
    Register * r2 = find_register(opc_p->p[1]);

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
    unsigned int params[] = { REGISTER_ADDRESS_SIZE, VALUE_SIZE }; 
    read_n_command_parameters(2, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("cmpi", "reg %"PRIu64" and %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif 
    
    Register * r1 = find_register(opc_p->p[0]);

    if(r1->value > opc_p->p[1]) {
        akku = 1;
    }
    else if(r1->value < opc_p->p[1]) {
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
    unsigned int params[] = { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE };
    read_n_command_parameters(2, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("eql", "reg %"PRIu64" and reg %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif 
    
    akku = find_register(opc_p->p[0])->value == find_register(opc_p->p[0])->value;

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
    unsigned int params[] = { REGISTER_ADDRESS_SIZE, VALUE_SIZE };
    read_n_command_parameters(2, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("eqli", "reg %"PRIu64" and %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif 
    
    akku = find_register(opc_p->p[0])->value == opc_p->p[1];

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
    unsigned int params[] = { REGISTER_ADDRESS_SIZE };
    read_n_command_parameters(1, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("push", "reg %"PRIu64, opc_p->p[0]);
#endif

    stackpush(stack, &(find_register(opc_p->p[0])->value), VALUE_SIZE) ;
    
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
    if( stack_is_empty( stack ) ) {
        FATAL_DESC_ERROR("Cannot pop from empty stack!");
    }
    unsigned int params[] = { REGISTER_ADDRESS_SIZE };
    read_n_command_parameters(1, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("pop", "reg %"PRIu64, opc_p->p[0]);
#endif

    find_register(opc_p->p[0])->value = *((uint64_t*)stackpop(stack));
    
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
    unsigned int params[] = { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE };
    read_n_command_parameters(2, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("add", "reg %"PRIu64" + reg %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif

    akku = find_register(opc_p->p[0])->value + find_register(opc_p->p[1])->value;

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
    unsigned int params[] = { REGISTER_ADDRESS_SIZE, VALUE_SIZE };
    read_n_command_parameters(1, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("addi", "reg %"PRIu64" + %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif

    akku = find_register(opc_p->p[0])->value + opc_p->p[1];

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
    unsigned int params[] = { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE };
    read_n_command_parameters(2, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("addr", "reg %"PRIu64" + reg %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif

    find_register(opc_p->p[0])->value += find_register(opc_p->p[1])->value;

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
    unsigned int params[] = { REGISTER_ADDRESS_SIZE, VALUE_SIZE };
    read_n_command_parameters(2, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("addir", "reg %"PRIu64" + %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif

    find_register(opc_p->p[0])->value += opc_p->p[1];

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
    unsigned int params[] = { PROGRAM_ADDRESS_SIZE };
    read_n_command_parameters(1, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("jmp", "to %"PRIu64, opc_p->p[0]);
#endif 

    if( minx_binary_exists_at(opc_p->p[0]) || opc_p->p[0] == END_OF_PROGRAM) {
        program_pointer = opc_p->p[0];
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
    unsigned int params[] = { REGISTER_ADDRESS_SIZE, PROGRAM_ADDRESS_SIZE };
    read_n_command_parameters(2, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("jmpiz", "to %"PRIu64" if reg %"PRIu64" == 0", opc_p->p[1], opc_p->p[0]);
#endif 

    if( minx_binary_exists_at(opc_p->p[1]) || opc_p->p[1] == END_OF_PROGRAM) {
        if( find_register(opc_p->p[0])->value == 0x0000 ) {
            program_pointer = opc_p->p[1];
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
    unsigned int params[] = { REGISTER_ADDRESS_SIZE, PROGRAM_ADDRESS_SIZE }; 
    read_n_command_parameters(2, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("jmpnz", "to %"PRIu64" if reg %"PRIu64" != 0", opc_p->p[1], opc_p->p[0]);
#endif 

    if( minx_binary_exists_at(opc_p->p[1]) || opc_p->p[1] == END_OF_PROGRAM) {
        if( find_register(opc_p->p[0])->value != 0x0000 ) {
            program_pointer = opc_p->p[1];
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
    unsigned int params[] = { PROGRAM_ADDRESS_SIZE, PROGRAM_ADDRESS_SIZE }; 
    read_n_command_parameters(2, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("jmpnz", "to %"PRIu64" if akku == 0, else to %"PRIu64, opc_p->p[0], opc_p->p[1]);
#endif 

    if( (minx_binary_exists_at(opc_p->p[0]) || opc_p->p[0] == END_OF_PROGRAM) && 
        (minx_binary_exists_at(opc_p->p[1]) || opc_p->p[1] == END_OF_PROGRAM)) {

        if( akku == 0x0000 ) {
            program_pointer = opc_p->p[0];
        }
        else {
            program_pointer = opc_p->p[1];
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

/*
 * Command:                 PMEMS
 * Parameters:              0
 * Affects Program Pointer: NO 
 */
static void opc_pmems_func(void) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("pmems");

    if( minx_config_get(CONF_SRC_DEBUGGING)->b ) {
        uint64_t i;
        unsigned int line = 0, j = 0;
        for( i = 0 ; i < heapnodes_count; i++ ) {
            printf("M: %"PRIu64"\n", i);
            printf("0x00000000 : ");
            for(j = 0; j < heapnodes[i].size; j++ ) {
                if( heapnodes[i].memory[j] == 0 ) {
                    printf("0x00 ");
                }
                else {
                    printf("%#02x ", heapnodes[i].memory[j]);
                }

                if((j+1) % 8 == 0) {
                    printf("\n%#010x : ", ++line);
                }
            }
            printf("\n");
        }
        printf("\n");
    }
#endif
    program_pointer += OPC_SIZE;
}

/*
 * Command:                 PMEM 
 * Parameters:              1, heapnode-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_pmem_func(void) {
#ifdef DEBUGGING
    if( !minx_config_get(CONF_SRC_DEBUGGING)->b ) {
        EXPLAIN_OPCODE("pmem");
    }
    else {
        unsigned int params[] = { REGISTER_ADDRESS_SIZE };
        read_n_command_parameters(1, params);

        EXPLAIN_OPCODE_WITH("pmem", "memory: %"PRIu64, registers[opc_p->p[0]].value);

        HeapNode *h = find_heapnode(registers[opc_p->p[0]].value);
        uint64_t i;
        unsigned int line = 0;
        printf("0x00000000 : ");
        for( i = 0 ; i < h->size; i++ ) {
            if( h->memory[i] == 0 ) {
                printf("0x00 ");
            }
            else {
                printf("%#02x ", h->memory[i]);
            }

            if((i+1) % 8 == 0) {
                printf("\n%#010x : ", ++line);
            }
        }
        printf("\n");
    }

#endif
    program_pointer += (OPC_SIZE + REGISTER_ADDRESS_SIZE);
}


/*
 * Command:                 ALLOC 
 * Parameters:              1, register-address
 * Affects Program Pointer: NO
 *
 * Breaking DRY rule here, as opc_alloci_func() is almost the same
 *
 */
static void opc_alloc_func(void) {

    HeapNode    *h;
    char        *memory;
    unsigned int params[] = { REGISTER_ADDRESS_SIZE };
    read_n_command_parameters(1, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("alloc", "%"PRIu64" Bytes", opc_p->p[0]);
#endif

    memory = (char*) malloc(opc_p->p[0]);
    if( memory == NULL || !(h = find_or_create_unused_heapnode()) ) {
        clrbit(statusregister, ALLOC_BIT);
        akku = (uint64_t)0x00;
    }
    else {
        h->used         = HEAPNODE_USED;
        h->size         = opc_p->p[0];
        h->real_size    = opc_p->p[0];
        h->memory       = memory;

        setbit(statusregister, ALLOC_BIT);
        akku = h->first_byte_addr;
    }

    program_pointer += (OPC_SIZE + REGISTER_ADDRESS_SIZE);
}

/*
 * Command:                 ALLOCI
 * Parameters:              1, value
 * Affects Program Pointer: NO
 *
 * Breaking DRY rule here, as opc_alloc_func() is almost the same
 *
 */
static void opc_alloci_func(void) {

    HeapNode    *h;
    void        *memory;
    unsigned int params[] = { VALUE_SIZE };
    read_n_command_parameters(1, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("alloci", "%"PRIu64" Bytes", opc_p->p[0]);
#endif

    memory = (char*) malloc(opc_p->p[0]);
    if( memory == NULL || !(h = find_or_create_unused_heapnode()) ) {
        clrbit(statusregister, ALLOC_BIT);
        akku = (uint64_t)0x00;
    }
    else {
        h->used         = HEAPNODE_USED;
        h->size         = opc_p->p[0];
        h->real_size    = opc_p->p[0];
        h->memory       = memory;

        setbit(statusregister, ALLOC_BIT);
        akku = h->first_byte_addr;
    }
    
    program_pointer += (OPC_SIZE + REGISTER_ADDRESS_SIZE);
}


/*
 * Command:                 RESIZE 
 * Parameters:              1, heap-address, register-address 
 * Affects Program Pointer: NO
 *
 * Breaking DRY rule here, as opc_resizei_func() is almost the same
 *
 */
static void opc_resize_func(void) {
    unsigned int params[] = { HEAP_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE };
    read_n_command_parameters(2, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("resize", "heap %"PRIu64" to %"PRIu64" Bytes", 
            opc_p->p[0], opc_p->p[1]);
#endif 

    HeapNode *h = find_heapnode(opc_p->p[0]);
    if( h == NULL ) {
        FATAL_DESC_ERROR("Memory could not be accessed");
    }

    if( opc_p->p[1] == h->size ){ 
        clrbit(statusregister, RESIZE_BIT);
        return;
    }

    /*
     * if the resize value is smaller as the actual size or the _real_ size,
     * just set the size variable in the heapnode
     */
    if( opc_p->p[1] <= h->size || opc_p->p[1] <= h->real_size) {
        h->size = opc_p->p[1];
    }
    /*
     * if the parameter says, the memory should be resized, do it!
     */
    else {
        h->memory = (char*) realloc(h->memory, opc_p->p[1]);
        h->real_size = h->size = opc_p->p[1];
    }

    setbit(statusregister, RESIZE_BIT);

    program_pointer += (OPC_SIZE + HEAP_ADDRESS_SIZE + REGISTER_ADDRESS_SIZE);
}

/*
 * Command:                 RESIZEI
 * Parameters:              1, heap-address, value 
 * Affects Program Pointer: NO
 *
 * Breaking DRY rule here, as opc_resize_func() is almost the same
 *
 */
static void opc_resizei_func(void) {
    unsigned int params[] = { HEAP_ADDRESS_SIZE, VALUE_SIZE };
    read_n_command_parameters(2, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("resizei", "heap %"PRIu64" to %"PRIu64" Bytes", 
            opc_p->p[0], opc_p->p[1]);
#endif 

    HeapNode *h = find_heapnode(opc_p->p[0]);
    if( h == NULL ) {
        FATAL_DESC_ERROR("Memory could not be accessed");
    }

    if( opc_p->p[1] == h->size ){ 
        clrbit(statusregister, RESIZE_BIT);
        return;
    }

    /*
     * if the resize value is smaller as the actual size or the _real_ size,
     * just set the size variable in the heapnode
     */
    if( opc_p->p[1] <= h->size || opc_p->p[1] <= h->real_size) {
        h->size = opc_p->p[1];
    }
    /*
     * if the parameter says, the memory should be resized, do it!
     */
    else { //if( opc_p->p[2] > h->size ) {
        h->memory = (char*) realloc(h->memory, opc_p->p[1]);
        h->real_size = h->size = opc_p->p[1];
    }
        
    setbit(statusregister, RESIZE_BIT);

    program_pointer += (OPC_SIZE + HEAP_ADDRESS_SIZE + VALUE_SIZE);
}

/*
 * Command:                 FREE 
 * Parameters:              1, heap-address
 * Affects Program Pointer: NO
 *
 * This opcode does not really remove the memory, it just marks it as unused. So
 * if later on memory is required, this one can be used.
 *
 */
static void opc_free_func(void) {
    unsigned int params[] = { HEAP_ADDRESS_SIZE };
    read_n_command_parameters(1, params);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("free", "heap %"PRIu64, opc_p->p[0]);
#endif
    
    HeapNode *h         = find_heapnode(registers[opc_p->p[0]].value);
    h->used             = HEAPNODE_NOT_USED;
    h->memory           = (char*) memset(h->memory, 0x00, h->real_size);

    program_pointer += (OPC_SIZE + HEAP_ADDRESS_SIZE);
}

/*
 * Command:                 PUT
 * Parameters:              3, heap-address, register-address, register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_put_func(void) {
    unsigned int params[] = {   HEAP_ADDRESS_SIZE, 
                                REGISTER_ADDRESS_SIZE, 
                                REGISTER_ADDRESS_SIZE };
    read_n_command_parameters(3, params);

    HeapNode        *h;
    char            *mem;
    uint64_t        mask;
    uint64_t        masked_bytes;

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("put", 
            "into heap %"PRIu64" val of reg %"PRIu64"(%"PRIu64" Bytes)", 
            opc_p->p[0], 
            opc_p->p[1],
            registers[opc_p->p[2]].value
            );
#endif 

    /*
     * A register has 8 Byte. So, we can only put 8 byte at once. 
     * I will not read more than one register to put, because it is so ugly!
     */
    if( registers[opc_p->p[2]].value > (uint64_t)REGISTER_SIZE ) {
        FATAL_DESC_ERROR("Cannot read more than 8 Byte from register");
    }

    h   = find_heapnode(registers[opc_p->p[0]].value);
    mem = get_memory_from_heapnode(h, registers[opc_p->p[0]].value);

    /*
     * Build the mask to mask the bytes from heap, so in the register there is
     * just the data from heap and not the artifacts from prior use!
     */
    for(mask = 0x00, masked_bytes = registers[opc_p->p[2]].value; masked_bytes; masked_bytes--) {
        mask = (mask<<8) | 0xFF;
    }

    memcpy(&registers[opc_p->p[1]].value, mem, registers[opc_p->p[2]].value);
    registers[opc_p->p[1]].value &= mask;

    program_pointer += (OPC_SIZE + HEAP_ADDRESS_SIZE + REGISTER_ADDRESS_SIZE);
}

/*
 * Command:                 READ
 * Parameters:              3, heap-address, register-address, register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_read_func(void) {
    unsigned int params[] = {   HEAP_ADDRESS_SIZE,
                                REGISTER_ADDRESS_SIZE,
                                REGISTER_ADDRESS_SIZE };
    read_n_command_parameters(3, params);

    HeapNode    *h;
    char        *mem;

#ifdef DEBUGGING 
    EXPLAIN_OPCODE_WITH("read",
            "from heap %"PRIu64" %"PRIu64" Byte into reg %"PRIu64,
            opc_p->p[0],
            registers[opc_p->p[2]].value,
            opc_p->p[1]
            );
#endif 

    /*
     * A register has 8 Byte. So, we can only put 8 byte at once. 
     * I will not read more than one register to put, because it is so ugly!
     */
    if( registers[opc_p->p[2]].value > (uint64_t)REGISTER_SIZE ) {
        FATAL_DESC_ERROR("Cannot read more than 8 Byte from register");
    }
    else if ( registers[opc_p->p[2]].value != 0 ) {
        h   = find_heapnode(registers[opc_p->p[0]].value);
        mem = get_memory_from_heapnode(h, opc_p->p[0]);

        /*
         * set the target register to 0x00, so the previous stored data is
         * completely gone when setting the read data from the heap.
         */
        registers[opc_p->p[1]].value = 0x00;
        memcpy(&registers[opc_p->p[1]].value, mem, registers[opc_p->p[2]].value);
    }
    /*
     * else do nothing (if the size is set to zero) 
     */

}

/*
 * Command:                 GETSIZE
 * Parameters:              1, heap-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_getsize_func(void) {
    unsigned int params[] = { HEAP_ADDRESS_SIZE };
    read_n_command_parameters(1, params);

    HeapNode *h;

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("GETSIZE", "of heap %"PRIu64" into akku", opc_p->p[0]);
#endif 

    h       = find_heapnode(registers[opc_p->p[0]].value);
    akku    = h->size; 

    program_pointer += (OPC_SIZE + HEAP_ADDRESS_SIZE);
}

#if (defined VERBOSITY | defined DEBUGGING)
static void print_register(unsigned int i) {
    printf( MINX_VPU_REGISTER_PREFIX"[%03i] = %"PRIu64"\n", i, registers[i].value );
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
