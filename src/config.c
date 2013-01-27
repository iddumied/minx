#include "config.h"

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
static Configuration    configuration[] = {
    {   .type = CONF_VERBOSITY,             .value.b = 0    },
    {   .type = CONF_MVPU_DEBUGGING,        .value.b = 0    },
    {   .type = CONF_SRC_DEBUGGING,         .value.b = 0    },
    {   .type = CONF_PRINT_REGS_AT_EOP,     .value.b = 0    },
    {   .type = CONF_DISASM,                .value.b = 0    },
};

/*
 * config keys 
 */
static ConfigurationKeyMap confkeys[] = {
    {   .type   = CONF_VERBOSITY,               .configkey = "-v"           },
    {   .type   = CONF_MVPU_DEBUGGING,          .configkey = "--minx-debug" },
    {   .type   = CONF_SRC_DEBUGGING,           .configkey = "-d"           },
    {   .type   = CONF_PRINT_REGS_AT_EOP,       .configkey = "--regs"       },
    {   .type   = CONF_DISASM,                  .configkey = "--disasm"     },
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
            if( 0 == strcmp( confkeys[j].configkey, argv[i] )) {
                /*
                switch(confkeys[j].type) {
                    case CONF_VERBOSITY:
                    case CONF_MVPU_DEBUGGING:
                    case CONF_SRC_DEBUGGING:
                    case CONF_PRINT_REGS_AT_EOP:
                    */


                /* currently, there are only bool configs, but later on there
                 * maybe are some more, key-value-pair type configurations
                 */
                        set_config(confkeys[j].type);
                        /*
                        break;
                }
                */
            }
        }
    }

    /*
     * here, the config is parsed. So do the checks
     */
#if (defined DEBUG | defined DEBUGGING)
    ConfigurationValue *cv = minx_config_get(CONF_MVPU_DEBUGGING);
    if( cv != NULL && cv->b ) {
        printf("[minx][conf]: parsing args\n");
        print_config();
    }
#endif 
}

ConfigurationValue* minx_config_get(ConfigurationType ct) {
    unsigned int i;
    for(i = 0 ; i < (sizeof(configuration)/sizeof(configuration[0])); i++)
        if(configuration[i].type == ct)
            return &configuration[i].value;
    return NULL;
}

int minx_config_is_set(ConfigurationType ct) {
    return minx_config_get(ct) != NULL;
}

/*
 *
 * Static functions
 *
 */
static void set_config(ConfigurationType ct) {
#if (defined DEBUG | defined DEBUGGING)
    ConfigurationValue *cv = minx_config_get(CONF_MVPU_DEBUGGING);
    if( cv != NULL && cv->b ) {
        printf(MINX_CONFIG_PREFIX": conf set\n");
    }
#endif 

    unsigned int i;
    for(i = 0;
        i < (sizeof(configuration)/sizeof(configuration[0])) &&
        configuration[i].type != ct;
        i++ );
    configuration[i].value.b = 1;
}

#if (defined DEBUG | defined DEBUGGING)
static void print_config() {
    unsigned int i;
    for( i = 0 ; i < (sizeof(configuration)/sizeof(configuration[0])); i++) {
        printf(MINX_CONFIG_PREFIX": config %s = %i\n", confkeys[i].configkey, configuration[i].value.b);
    }
}
#endif
