#include "kernel/opcodes.h"

/*
 * Command:                 RAND
 * Parameters:              0:
 * Affects Program Pointer: NO
 *
 *
 */
#ifndef DESTROY_MY_APPLICATION
void minx_opc_rand_func(uint64_t *params) {

/* // there is no explain for a random. It's just random!
#ifdef DEBUGGING
    EXPLAIN_OPCODE();
#endif
*/

    Register *akku = minx_registers_find_register(akku_register_number);
    Register *status = minx_registers_find_register(statusregister);

    int fd;
    if (( fd = open("/dev/random", O_RDONLY)) == -1 ) {
        setbit(status->value, NO_RAND);
    }
    else {
        read(fd, &(akku->value), REGISTER_SIZE);
        close(fd);
    }
}
#else //DESTROY_MY_APPLICATION

/*
 * Sony-style implementation of rand()
 */
void minx_opc_rand_func(uint64_t *params) {
    minx_registers_find_register(akku_register_number)->value = 5;
    /* determined by fair dice-roll ! */
}
#endif //DESTROY_MY_APPLICATION 
