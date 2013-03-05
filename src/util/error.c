#include "error.h"

static ShutdownFunctionPtr  *shutdown_functions;
static unsigned int         shutdown_functions_cnt;

/**
 * @brief Init the error module
 */
void minx_error_init() {
    shutdown_functions      = (ShutdownFunctionPtr*) malloc(sizeof(ShutdownFunctionPtr));
    shutdown_functions_cnt  = 0;

    minx_error_register_shutdown_function(minx_error_shutdown);
}

/**
 * @brief shutdown function for the error module
 */
void minx_error_shutdown() {
    free(shutdown_functions);
}

/**
 * @brief Shutdown all modules (global shutdown function)
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

/**
 * @brief Register a function as shutdown function
 *
 * @param func The function to register
 */
void minx_error_register_shutdown_function(void (*func)(void)) {
    shutdown_functions = (ShutdownFunctionPtr*) realloc(shutdown_functions, 
            sizeof(ShutdownFunctionPtr) * (shutdown_functions_cnt+1));

    shutdown_functions[shutdown_functions_cnt] = func;
    shutdown_functions_cnt++;
}
