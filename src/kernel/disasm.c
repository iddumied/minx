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

    /*
     * This is not a "magic" constant. It is just... I cannot calculate the
     * length of the longest string representation of a opcode, as the type
     * OpcodeInformation is incomplete, as I use a array.
     *
     * So I try it with 10. If it does not fit, it only disturbes the output a
     * bit. Not so bad.
     */
    unsigned int longest_strrep_len = 10;
    unsigned int curr_strrep_len;

    unsigned int progp_space;
#define PROG_P_SPACE 6

    OpcodeInformation opci;

    while(minx_binary_exists_at(program_pointer)) {
        opcode = minx_binary_get_at(program_pointer, OPC_SIZE, opcode, sizeof(*opcode));

        opci = opcodes[*opcode];

        if(in_hex) {
            printf("[%#010"PRIx64"]", program_pointer);
        }
        else {
            progp_space = printf("[%"PRIu64"]", program_pointer);
            while(progp_space < PROG_P_SPACE) {
                printf(" ");
                progp_space++;
            }
        }

        curr_strrep_len = strlen(opci.strrep);
        printf(" %s", opci.strrep);
        while(curr_strrep_len < longest_strrep_len) {
            printf(" ");
            curr_strrep_len++;
        }

        program_pointer += OPC_SIZE;
        print_parameters(opci.params);
    }

#undef PROG_P_SPACE
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

    int printed_chars;
#define PRINT_PARAM_SPACE 8

    int in_hex = minx_config_get(CONF_HEX)->b;

    for(i = 0; params[i] || i < MAX_PARAMETER_COUNT; i++) {
        printf(" %u: ", i);

        if( params[i] == REGISTER_ADDRESS_SIZE || params[i] == HEAP_ADDRESS_SIZE ) {
            param16bit = (uint16_t*) minx_binary_get_at(program_pointer, 
                                                        params[i],
                                                        param16bit,
                                                        params[i]);

            if(in_hex) {
                if (*param16bit == 0x00)
                    printf("0x0000000000000000");
                else 
                    printf("%#018"PRIx16, *param16bit);
            }
            else {
                printed_chars = printf("%"PRIu16, *param16bit);
                while(printed_chars < PRINT_PARAM_SPACE) {
                    printf(" ");
                    printed_chars++;
                }
            }
        }
        else { /* param 8 byte = 64 bit */
            param64bit = (uint64_t*) minx_binary_get_at(program_pointer,
                                                        params[i],
                                                        param64bit,
                                                        params[i]);

            if(in_hex) {
                if (*param64bit == 0x00)
                    printf("0x0000000000000000");
                else 
                    printf("%#018"PRIx64, *param64bit);
            }
            else {
                printed_chars = printf("%"PRIu64, *param64bit);
                while(printed_chars < PRINT_PARAM_SPACE) {
                    printf(" ");
                    printed_chars++;
                }
            }
        }

        program_pointer += params[i];
    }
    printf("\n");

#undef PRINT_PARAM_SPACE
}

#endif // DISASSEMBLE
