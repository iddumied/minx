#include "config.h"

/*
 * Static variables
 */
static Configuration    configuration[] = {
    {   .type = VERBOSITY,  .value.i = 0  },
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
 *
 */
void minx_config_parse(unsigned int len, char *str) {
}

void * minx_config_get(ConfigurationType ct) {
    unsigned int i;
    for(i = 0 ; i < number_of_confs; i++)
        if(configuration[i].type == ct)
            return &configuration[i].value;
    return NULL;
}
