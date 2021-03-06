#include "kernel/opcodes.h"

/**
 * @brief Opcode URAND
 *
 * Command:                 URAND
 * Parameters:              0:
 * Affects Program Pointer: NO
 *
 *
 *
 * @param params Pointer to parameters for this opcode
 */
#ifndef DESTROY_MY_APPLICATION
void minx_opc_urand_func(uint64_t *params) {

    Register *akku = minx_registers_find_register(akku_register_number);
    Register *status = minx_registers_find_register(statusregister);

    int fd;
    if (( fd = open("/dev/urandom", O_RDONLY)) == -1 ) {
        setbit(status->value, NO_RAND);
    }
    else {
        read(fd, &(akku->value), REGISTER_SIZE);
        close(fd);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF("%#010"PRIx64, "%"PRIu64, akku->value);
#endif

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
