#include "error.h"

typedef void(*ShutdownFunctionPtr)(void);
static ShutdownFunctionPtr  *shutdown_functions;
static unsigned int         shutdown_functions_cnt;

/*
 * init function for the error module
 */
void minx_error_init() {
    shutdown_functions      = (ShutdownFunctionPtr*) malloc(sizeof(ShutdownFunctionPtr));
    shutdown_functions_cnt  = 0;

    minx_error_register_shutdown_function(minx_error_shutdown);
}

/*
 * shutdown function for the error module
 */
void minx_error_shutdown() {
}

/*
 * shutdown all modules.
 *
 * counts down, so last registered shutdown function is called first.
 * That's good because the shutdown function for the error-module is registered
 * first!
 */
void minx_error_global_shutdown(void) {
    unsigned int i;
    for(i = shutdown_functions_cnt; i ; i-- ) {
        shutdown_functions[i-1]();
    }
}

/*
 * register shutdown function
 */
void minx_error_register_shutdown_function(void (*func)(void)) {
    shutdown_functions = (ShutdownFunctionPtr*) realloc(shutdown_functions, 
            sizeof(ShutdownFunctionPtr) * (shutdown_functions_cnt+1));

    shutdown_functions[shutdown_functions_cnt] = func;
    shutdown_functions_cnt++;
}
