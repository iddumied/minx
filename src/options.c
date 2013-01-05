#include "options.h"

static char* opts;

void minx_option_init(void) {
    /*
     * malloc options array with size for 8 options
     */
    opts = malloc(1 * sizeof(char));
}

void minx_option_set(signed int num) {
    *opts &= 0x01 << num;
}

int minx_option_is_set(signed int num) {
    return *opts | num;
}

void minx_option_destroy(void) {
    free(opts);
}
