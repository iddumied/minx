#include "config.h"

/*
 * static functions
 */
static void         set_config          (ConfigurationType ct);

/*
 * Static variables
 */

/*
 * default config 
 */
static Configuration    configuration[] = {
    {   .type = CONF_VERBOSITY,             .value.i = 0    },
    {   .type = CONF_MVM_DEBUGGING,         .value.i = 0    },
    {   .type = CONF_SRC_DEBUGGING,         .value.i = 0    },
    {   .type = CONF_PRINT_REGS_AT_EOP,     .value.i = 0    },
};

/*
 * config keys 
 */
static ConfigurationKeyMap confkeys[] = {
    {   .type   = CONF_VERBOSITY,               .configkey = "-v"           },
    {   .type   = CONF_MVM_DEBUGGING,           .configkey = "--vm-debug"   },
    {   .type   = CONF_SRC_DEBUGGING,           .configkey = "-d"           },
    {   .type   = CONF_PRINT_REGS_AT_EOP,       .configkey = "--regs"       },
};

/*
 * Functions
 */

void minx_config_init() {
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
        for( j = 0 ; j < (sizeof(*confkeys)/sizeof(confkeys[0])); j++ ) {
            if( 0 == strcmp( confkeys[j].configkey, argv[i] )) {
                /*
                switch(confkeys[j].type) {
                    case CONF_VERBOSITY:
                    case CONF_MVM_DEBUGGING:
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
}

void * minx_config_get(ConfigurationType ct) {
    unsigned int i;
    for(i = 0 ; i < (sizeof(*configuration)/sizeof(configuration[0])); i++)
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
    unsigned int i;
    for(i = 0;
        i < (sizeof(*configuration)/sizeof(configuration[0])) &&
        configuration[i].type != ct;
        i++ );
    configuration[i].value.i = 1;
}
