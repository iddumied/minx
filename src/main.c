#include "main.h"
#include "config.h"
#include "binary_reader.h"
#include "vpu.h"

#ifdef DISASSEMBLE
#include "disasm.h"
#endif // DISASSEMBLE

#include <stdlib.h>
#include <stdio.h>

void handle_signal(int signal) {
    if( signal == SIGINT || signal == SIGKILL ) {
        printf("[%i]: Shutdown ...", signal);
        minx_binary_shutdown();
        minx_config_shutdown();
        minx_vpu_shutdown();
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

    minx_config_init();
    minx_config_parse(argc, args);
    minx_binary_init(f);

#ifdef DISASSEMBLE
    if( minx_config_get(CONF_DISASM)->b ) {
        minx_disasm_run();
    }
    else {
        minx_vpu_run();
    }
#else /* for the readability */
    minx_vpu_run();
#endif //DISASSEMBLE


    minx_binary_shutdown();
    minx_config_shutdown();

    fclose(f);
    return 0;
}
