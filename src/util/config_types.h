#ifndef __MINX_CONFIG_TYPES_H__
#define __MINX_CONFIG_TYPES_H__

/*
 * The configuration types are outsourced, because the module.h header has to
 * include them, too. But not the function-prototypes!
 */

typedef enum {
    CONF_VERBOSITY,         /* only if compiled with DEBUG */
    CONF_MINX_DEBUGGING,    /* only if compiled with DEBUGGING */
    CONF_SRC_DEBUGGING,     /* only if compiled with DEBUGGING */
    CONF_PRINT_REGS_AT_EOP, /* print registers at end of program */
    CONF_PRINT_REGS_AT_EOP_WITHOUT_ZEROVALREGS, /* print registers at end of 
                                                   program, but not them with 
                                                   value==0 */
    CONF_DISASM,            /* only start diassembler */
    CONF_HEX,               /* print always in hex */
    CONF_FAST,              /* try to be faster, although there is more mem required */
} ConfigurationType;

typedef union {
    int b;
    int i; /* just for the convention as 'b' is for bool and 'i' for integer */
    char *str;
} ConfigurationValue;

#endif //__MINX_CONFIG_TYPES_H__
