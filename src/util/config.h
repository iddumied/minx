#ifndef __MINX_CONFIG_H__
#define __MINX_CONFIG_H__

#include <stdlib.h>
#include <string.h>

#include "util/error.h"
#include "util/debug.h"

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
    CONF_HELP,              /* print help and exit */
    CONF_HELP_SHORT,        /* print help and exit (short code) */
} ConfigurationType;

typedef union {
    int b;
    int i;
    char *str;
} ConfigurationValue;

void                    minx_config_init    (void);
void                    minx_config_shutdown(void);
void                    minx_config_parse   (unsigned int argc, char ** argv);
ConfigurationValue*     minx_config_get     (ConfigurationType ct);
int                     minx_config_is_set  (ConfigurationType ct);

#endif // __MINX_CONFIG_H__
