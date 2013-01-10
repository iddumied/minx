#include "binary_reader.h"
#include "vm.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **args) {
    if( argc != 2 ) {
        printf("No Binary given!\n");
        exit(1);
    }
    FILE *f = fopen(args[1], "r");

    minx_binary_init(f);
    minx_vm_run();
    minx_binary_shutdown();
    return 0;
}
