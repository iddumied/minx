#include "main.h"

void handle_signal(int signal) {
    if( signal == SIGINT || signal == SIGKILL ) {
        printf("[%i]: Shutdown ...", signal);
        minx_error_global_shutdown();
        printf("done.\n");
    }
}

int main(int argc, char **args) {
    if( argc < 2 ) {
        printf("No Binary given!\n");
        exit(1);
    }

    signal(SIGINT, handle_signal);
    signal(SIGQUIT, handle_signal);

    FILE *f = fopen(args[1], "r");

    /*
    char * file = "./out.out";
    FILE *f = fopen(file, "r");
    */

    int exit_code;

    minx_error_init();
    minx_config_init();
    minx_config_parse(argc, args);

    if(minx_config_get(CONF_MINX_DEBUGGING)->b) {
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
    return exit_code;
}
