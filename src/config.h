#ifndef __MINX_CONFIG_H__
#define __MINX_CONFIG_H__

#include <stdlib.h>
#include <string.h>

typedef enum {
    CONF_VERBOSITY,         /* only if compiled with DEBUG */
    CONF_MVM_DEBUGGING,     /* only if compiled with DEBUGGING */
    CONF_SRC_DEBUGGING,     /* only if compiled with DEBUGGING */
    CONF_PRINT_REGS_AT_EOP, /* print registers at end of program */
} ConfigurationType;

typedef union {
    int b;
    int i;
    char *str;
} ConfigurationValue;

typedef struct {
    ConfigurationType       type;
    ConfigurationValue      value;
} Configuration;

typedef struct {
    ConfigurationType       type;
    char                    *configkey;
} ConfigurationKeyMap;

void        minx_config_init    (void);
void        minx_config_shutdown(void);
void        minx_config_parse   (unsigned int argc, char ** argv);
void *      minx_config_get     (ConfigurationType ct);
int         minx_config_is_set  (ConfigurationType ct);

#endif // __MINX_CONFIG_H__
