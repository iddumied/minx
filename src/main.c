#include "main.h"

/**
 * @brief handle the signal passed by argument
 *
 * Currently, this function handles only SIGINT or SIGKILL and raises a global
 * shutdown after printing the signal message.
 *
 * @param signal The shutdown signal
 */
void handle_signal(int signal) {
    if( signal == SIGINT || signal == SIGKILL ) {
        printf("[%i]: Shutdown ...", signal);
        minx_error_global_shutdown();
        printf("done.\n");
    }
}

/**
 * @brief Main function
 *
 * @param argc Argument count
 * @param args Arguments. First must be binary
 *
 * @return Exit status of the binary
 */
int main(int argc, char **args) {
    if( argc < 2 ) {
        printf("No Arguments, invoking --help!\n\n");
        minx_print_help();
        exit(1);
    }

    signal(SIGINT, handle_signal);
    signal(SIGQUIT, handle_signal);
    signal(SIGKILL, handle_signal);

    FILE *f;
    int exit_code;

    minx_error_init();
    minx_config_init();
    minx_config_parse(argc, args);

    if(minx_config_get(CONF_HELP)->b || minx_config_get(CONF_HELP_SHORT)->b) {
        exit_code = 0;
        minx_print_help();
        goto end;
    }

    if( !minx_util_filecheck_file_exists(args[1]) || 
        !minx_util_filecheck_path_is_file(args[1]) ||
        !minx_util_filecheck_file_is_readable(args[1])){
    
        exit_code = 1;
        goto end;
    }

    f = fopen(args[1], "r");

    if(f == NULL) {
        handle_file_open_error();
        exit_code = 1;
        goto end;
    }

    if(minx_config_get(CONF_MINX_DEBUGGING)->b) {
        printminxasciiart();
        mtrace();
    }

    minx_binary_init(f);

#ifdef DISASSEMBLE
    if( minx_config_get(CONF_DISASM)->b ) {
        minx_disasm_run();
    }
    else {
        minx_kernel_init();
        exit_code = minx_kernel_run();
        minx_kernel_shutdown();
    }
#else /* for the readability */
    minx_kernel_init();
    exit_code = minx_kernel_run();
    minx_kernel_shutdown();
#endif //DISASSEMBLE


    minx_binary_shutdown();
    minx_config_shutdown();
    minx_error_shutdown();

    fclose(f);

end:
    return exit_code;
}
