#ifdef DISASSEMBLE

#include "disasm.h"

static void print_parameters(unsigned int *params);

static uint64_t program_pointer;

/**
 * @brief Runs the disassembler
 *
 * The function creates a own program pointer and runs through all opcodes, but
 * doesn't execute them. It just prints the string representation of the opcode
 * and it's parameters by calling the print_parameters() function.
 *
 */
void minx_disasm_run() {
    program_pointer = 0x00;
    uint16_t *opcode = (uint16_t*) malloc(sizeof(uint16_t));
    int in_hex = minx_config_get(CONF_HEX)->b;

    OpcodeInformation opci;

    while(minx_binary_exists_at(program_pointer)) {
        opcode = minx_binary_get_at(program_pointer, OPC_SIZE, opcode, sizeof(*opcode));

        opci = opcodes[*opcode];

        if(in_hex)
            printf("[%#010"PRIx64"]", program_pointer);
        else 
            printf("[%"PRIu64"]", program_pointer);

        printf(" %s", opci.strrep);
        program_pointer += OPC_SIZE;
        print_parameters(opci.params);
    }
}

/**
 * @brief Print passed parameters
 *
 * Prints the passed parameters based on configuration. Prints hex values if hex
 * is configured, else decimal.
 *
 * Maximal number of parameters is defined in def/sizes.h by macro.
 *
 * @param params Parameters to print
 */
static void print_parameters(unsigned int *params) {
    unsigned int i;
    uint16_t *param16bit = malloc( sizeof(uint16_t) );
    uint64_t *param64bit = malloc( sizeof(uint64_t) );

    int in_hex = minx_config_get(CONF_HEX)->b;

    for(i = 0; params[i] || i < MAX_PARAMETER_COUNT; i++) {
        printf(" %u: ", i);

        if( params[i] == REGISTER_ADDRESS_SIZE || params[i] == HEAP_ADDRESS_SIZE ) {
            param16bit = (uint16_t*) minx_binary_get_at(program_pointer, 
                                                        params[i],
                                                        param16bit,
                                                        params[i]);

            if(in_hex) {
                printf("%#010"PRIx16, *param16bit);
            }
            else {
                printf("%"PRIu16, *param16bit);
            }
        }
        else { /* param 8 byte = 64 bit */
            param64bit = (uint64_t*) minx_binary_get_at(program_pointer,
                                                        params[i],
                                                        param64bit,
                                                        params[i]);

            if(in_hex) {
                printf("%#010"PRIx64, *param64bit);
            }
            else {
                printf("%"PRIu16, *param16bit);
            }
        }
        
        if(params[i+1])
            printf(",");

        program_pointer += params[i];
    }
    printf("\n");
}

#endif // DISASSEMBLE
