#include "error.h"

static void (**shutdown_functions)(void);
static unsigned int shutdown_functions_cnt;

/*
 * init function for the error module
 */
void minx_error_init() {
    shutdown_functions      = (void(**)(void)) malloc(sizeof(*shutdown_functions));
    shutdown_functions_cnt  = 0;

    minx_error_register_shutdown_function(minx_error_shutdown);
}

/*
 * shutdown function for the error module
 */
void minx_error_shutdown() {
    free(shutdown_functions);
}

/*
 * shutdown all modules.
 *
 * counts down, so last registered shutdown function is called first.
 * That's good because the shutdown function for the error-module is registered
 * first!
 */
void minx_error_global_shutdown(void) {
    for(; shutdown_functions_cnt ; shutdown_functions_cnt-- ) {
        shutdown_functions[shutdown_functions_cnt-1]();
    }
}

/*
 * register shutdown function
 */
void minx_error_register_shutdown_function(void (*func)(void)) {
    shutdown_functions = realloc(shutdown_functions, 
            sizeof(*shutdown_functions) * shutdown_functions_cnt+1);

    shutdown_functions[shutdown_functions_cnt] = func;

    shutdown_functions_cnt++;
}
