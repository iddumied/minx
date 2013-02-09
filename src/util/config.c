#include "util/config.h"

/*
 * static functions
 */
static void         set_config          (ConfigurationType ct);

#if (defined DEBUG | defined DEBUGGING)
static void         print_config        (void);
#endif

/*
 * Static variables
 */

/*
 * default config 
 */
static ConfigurationValue configuration[] = {
    [CONF_VERBOSITY]            = { .b = 0 },
    [CONF_MINX_DEBUGGING]       = { .b = 0 },
    [CONF_SRC_DEBUGGING]        = { .b = 0 },
    [CONF_PRINT_REGS_AT_EOP]    = { .b = 0 },
    [CONF_DISASM]               = { .b = 0 },
    [CONF_HEX]                  = { .b = 0 },
};

/*
 * config keys 
 */
static char *confkeys[] = {
    [CONF_VERBOSITY]            = "-v",
    [CONF_MINX_DEBUGGING]       = "--minx-debug",
    [CONF_SRC_DEBUGGING]        = "-d",
    [CONF_PRINT_REGS_AT_EOP]    = "--regs",
    [CONF_DISASM]               = "--disasm",
    [CONF_HEX]                  = "--hex",
};

/*
 * Functions
 */

void minx_config_init() {
    minx_error_register_shutdown_function(minx_config_shutdown);
}

void minx_config_shutdown() {
    return;
}

/*
 * parse config 
 */
void minx_config_parse(unsigned int argc, char ** argv) {
    unsigned int i, j;
    for( i = 0 ; i < argc; i++ ) {
        for( j = 0 ; j < (sizeof(confkeys)/sizeof(confkeys[0])); j++ ) {
            if( 0 == strcmp(confkeys[j], argv[i])) {

                ConfigurationValue *cv = minx_config_get(CONF_MINX_DEBUGGING);
                if( cv != NULL && cv->b ) {
                    printf(MINX_CONFIG_PREFIX": conf set\n");
                }

                configuration[i].b = 1;
            }
        }
    }

    /*
     * here, the config is parsed. So do the checks
     */
#if (defined DEBUG | defined DEBUGGING)
    ConfigurationValue *cv = minx_config_get(CONF_MINX_DEBUGGING);
    if( cv != NULL && cv->b ) {
        printf("[minx][conf]: parsing args\n");
        print_config();
    }
#endif 
}

ConfigurationValue* minx_config_get(ConfigurationType ct) {
    if( (sizeof(configuration) / sizeof(ConfigurationValue)) <= ct) {
        return NULL;
    }
    else {
        return &configuration[ct];
    }
}

int minx_config_is_set(ConfigurationType ct) {
    return minx_config_get(ct) != NULL;
}

/*
 *
 * Static functions
 *
 */
#if (defined DEBUG | defined DEBUGGING)
static void print_config() {
    unsigned int i;
    for( i = 0 ; i < (sizeof(configuration)/sizeof(configuration[0])); i++) {
        printf(MINX_CONFIG_PREFIX": config %s = %i\n", confkeys[i], configuration[i].b);
    }
}
#endif
