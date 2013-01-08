#ifndef __MINX_CONFIG_H__
#define __MINX_CONFIG_H__

#include <stdlib.h>
#include <string.h>

typedef enum {
    VERBOSITY,
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

#endif // __MINX_CONFIG_H__
