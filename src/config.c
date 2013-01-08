#include "config.h"

/*
 * Static variables
 */

/*
 * default config 
 */
static Configuration    configuration[] = {
    {   .type = VERBOSITY,  .value.i = 0  },
};

/*
 * config keys 
 */
static ConfigurationKeyMap confkeys[] = {
    {   .type   = VERBOSITY,    .configkey = "-v" },
};

static unsigned int     number_of_confs;

/*
 * Functions
 */

void minx_config_init() {
    number_of_confs = (unsigned int) sizeof(configuration)/sizeof(configuration[0]);
}

void minx_config_shutdown() {
    return;
}

/*
 * parse config 
 */
void minx_config_parse(unsigned int argc, char ** argv) {

}

void * minx_config_get(ConfigurationType ct) {
    unsigned int i;
    for(i = 0 ; i < number_of_confs; i++)
        if(configuration[i].type == ct)
            return &configuration[i].value;
    return NULL;
}
