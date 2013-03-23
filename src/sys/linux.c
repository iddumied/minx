#include "sys/linux.h"

static struct sysinfo sys_info = NULL;

void minx_sys_init(void) {
    sys_info = (struct sysinfo*) malloc(sizeof(struct sysinfo));
}

void minx_sys_shutdown(void) {
    free(sys_info);
}

/**
 * @brief Get the RAM size of the machine
 *
 * @return The size of the RAM in bytes
 */
long minx_sys_get_ramsize(void) {
    if(total == 0)
        sysinfo(&sys_info);

    return sys_info->totalram;
}
