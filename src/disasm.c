#ifdef DISASSEMBLE

#include "disasm.h"

static void print_parameters(unsigned int *params);

static struct {
    char            *desc;

    /*
     *
     * Currently, maximal 4 parameters. This can be changed easily by changing
     * the array size! The compiler initializes all not-set elements with zero.
     * 
     */
    unsigned int    params[4];

} opcode_map[] = {
    [OPC_NOP]       = { "NOP",      { } }, 
    [OPC_CALL]      = { "CALL",     { PROGRAM_ADDRESS_SIZE } }, 
    [OPC_RET]       = { "RET",      { } }, 
    [OPC_MOV]       = { "MOV",      { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE } }, 
    [OPC_MOVI]      = { "MOVI",     { REGISTER_ADDRESS_SIZE, VALUE_SIZE} }, 
    [OPC_NOT]       = { "NOT",      { REGISTER_ADDRESS_SIZE } }, 
    [OPC_NOTR]      = { "NOTR",     { REGISTER_ADDRESS_SIZE } }, 
    [OPC_AND]       = { "AND",      { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE } }, 
    [OPC_ANDI]      = { "ANDI",     { REGISTER_ADDRESS_SIZE, VALUE_SIZE } }, 
    [OPC_ANDR]      = { "ANDR",     { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE } }, 
    [OPC_ANDIR]     = { "ANDIR",    { REGISTER_ADDRESS_SIZE, VALUE_SIZE } }, 
    [OPC_OR]        = { "OR",       { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE } }, 
    [OPC_ORI]       = { "ORI",      { REGISTER_ADDRESS_SIZE, VALUE_SIZE} }, 
    [OPC_ORR]       = { "ORR",      { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE } }, 
    [OPC_ORIR]      = { "ORIR",     { REGISTER_ADDRESS_SIZE, VALUE_SIZE } }, 
    [OPC_DEC]       = { "DEC",      { REGISTER_ADDRESS_SIZE } }, 
    [OPC_INC]       = { "INC",      { REGISTER_ADDRESS_SIZE } }, 
    [OPC_LSH]       = { "LSH",      { REGISTER_ADDRESS_SIZE } }, 
    [OPC_RSH]       = { "RSH",      { REGISTER_ADDRESS_SIZE } }, 
    [OPC_CMP]       = { "CMP",      { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE } }, 
    [OPC_CMPI]      = { "CMPI",     { REGISTER_ADDRESS_SIZE, VALUE_SIZE } }, 
    [OPC_EQL]       = { "EQL",      { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE } }, 
    [OPC_EQLI]      = { "EQLI",     { REGISTER_ADDRESS_SIZE, VALUE_SIZE } }, 
    [OPC_PUSH]      = { "PUSH",     { REGISTER_ADDRESS_SIZE } }, 
    [OPC_POP]       = { "POP",      { REGISTER_ADDRESS_SIZE } }, 
    [OPC_DROP]      = { "DROP",     { } }, 
    [OPC_ADD]       = { "ADD",      { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE } }, 
    [OPC_ADDI]      = { "ADDI",     { REGISTER_ADDRESS_SIZE, VALUE_SIZE } }, 
    [OPC_ADDR]      = { "ADDR",     { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE } }, 
    [OPC_ADDIR]     = { "ADDIR",    { REGISTER_ADDRESS_SIZE, VALUE_SIZE } }, 
    [OPC_JMP]       = { "JMP",      { PROGRAM_ADDRESS_SIZE } }, 
    [OPC_JMPIZ]     = { "JMPIZ",    { REGISTER_ADDRESS_SIZE, PROGRAM_ADDRESS_SIZE } }, 
    [OPC_JMPNZ]     = { "JMPNZ",    { REGISTER_ADDRESS_SIZE, PROGRAM_ADDRESS_SIZE } }, 
    [OPC_IFZJMP]    = { "IFZJMP",   { PROGRAM_ADDRESS_SIZE, PROGRAM_ADDRESS_SIZE } }, 
    [OPC_PSTACK]    = { "PSTACK",   { } }, 
    [OPC_PREGS]     = { "PREGS",    { } }, 
    [OPC_PPROG]     = { "PPROG",    { } }, 
    [OPC_PMEMS]     = { "PMEMS",    { } }, 
    [OPC_PMEM]      = { "PMEM",     { HEAP_ADDRESS_SIZE } }, 
    [OPC_ALLOC]     = { "ALLOC",    { REGISTER_ADDRESS_SIZE } }, 
    [OPC_ALLOCI]    = { "ALLOCI",   { VALUE_SIZE } }, 
    [OPC_RESIZE]    = { "RESIZE",   { HEAP_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE } }, 
    [OPC_RESIZEI]   = { "RESIZEI",  { HEAP_ADDRESS_SIZE, VALUE_SIZE } }, 
    [OPC_FREE]      = { "FREE",     { HEAP_ADDRESS_SIZE } }, 
    [OPC_PUT]       = { "PUT",      { HEAP_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE } }, 
    [OPC_READ]      = { "READ",     { HEAP_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE } }, 
    [OPC_GETSIZE]   = { "GETSIZE",  { HEAP_ADDRESS_SIZE } }, 
};

static uint64_t program_pointer;

void minx_disasm_run() {
    program_pointer = 0x00;
    uint16_t *opcode;
    unsigned int i;

    while(minx_binary_exists_at(program_pointer)) {
        opcode = minx_binary_get_at(program_pointer, OPC_SIZE, opcode);
        printf("%s", opcode_map[*opcode].desc);
        print_parameters(opcode_map[*opcode].params);
    }
}

static void print_parameters(unsigned int *params) {
    unsigned int i;
    for(i = 0; params[i]; i++) {
        printf("%"PRIu64, params[i]);
        if(params[i+1])
            printf(", ");
    }
    printf("\n");
}

#endif // DISASSEMBLE
