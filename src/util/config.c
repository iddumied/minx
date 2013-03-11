#include "util/config.h"

/*
 * static functions
 */
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
    [CONF_VERBOSITY]                                = { .b = 0 },
    [CONF_MINX_DEBUGGING]                           = { .b = 0 },
    [CONF_SRC_DEBUGGING]                            = { .b = 0 },
    [CONF_PRINT_REGS_AT_EOP]                        = { .b = 0 },
    [CONF_PRINT_REGS_AT_EOP_WITHOUT_ZEROVALREGS]    = { .b = 0 },
    [CONF_DISASM]                                   = { .b = 0 },
    [CONF_HEX]                                      = { .b = 0 },
    [CONF_FAST]                                     = { .b = 0 },
    [CONF_SAVE]                                     = { .b = 0 },
    [CONF_HELP]                                     = { .b = 0 },
    [CONF_HELP_SHORT]                               = { .b = 0 },
};

/*
 * config keys 
 */
static char *confkeys[] = {
    [CONF_VERBOSITY]                                = "-v",
    [CONF_MINX_DEBUGGING]                           = "--minx-debug",
    [CONF_SRC_DEBUGGING]                            = "-d",
    [CONF_PRINT_REGS_AT_EOP]                        = "--regs",
    [CONF_PRINT_REGS_AT_EOP_WITHOUT_ZEROVALREGS]    = "--notzeroregs",
    [CONF_DISASM]                                   = "--disasm",
    [CONF_HEX]                                      = "--hex",
    [CONF_FAST]                                     = "--fast",
    [CONF_SAVE]                                     = "--save",
    [CONF_HELP]                                     = "--help",
    [CONF_HELP_SHORT]                               = "-h"
};

/*
 * Functions
 */

/**
 * @brief Init the config
 */
void minx_config_init() {
    minx_error_register_shutdown_function(minx_config_shutdown);
}

/**
 * @brief shutdown the config
 */
void minx_config_shutdown() {
    return;
}

/*
 * parse config 
 */
/**
 * @brief Parse the passed config
 *
 * @param argc The number of arguments
 * @param argv The arguments
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

                configuration[j].b = 1;
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

/**
 * @brief Get the configuration of a specific type
 *
 * @param ct The configuration to get
 *
 * @return The configuration value
 */
ConfigurationValue* minx_config_get(ConfigurationType ct) {
    if( (sizeof(configuration) / sizeof(ConfigurationValue)) <= ct) {
        return NULL;
    }
    else {
        return &configuration[ct];
    }
}

/**
 * @brief Returns true if the passed config is set
 *
 * @param ct The config to check
 *
 * @return True if the config is set, else false
 */
int minx_config_is_set(ConfigurationType ct) {
    return minx_config_get(ct) != NULL;
}

/*
 *
 * Static functions
 *
 */
#if (defined DEBUG | defined DEBUGGING)
/**
 * @brief Print the config (human readable)
 */
static void print_config() {
    unsigned int i;
    for( i = 0 ; i < (sizeof(configuration)/sizeof(configuration[0])); i++) {
        printf(MINX_CONFIG_PREFIX": config %s = %i\n", confkeys[i], configuration[i].b);
    }
}
#endif
