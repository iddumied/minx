#include "sys/sys.h"

static struct sysinfo *sys_info;

void minx_sys_init(void) {
    minx_error_register_shutdown_function(minx_sys_shutdown);
    sys_info = (struct sysinfo*) malloc(sizeof(struct sysinfo));
    sysinfo(sys_info);
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
    return sys_info->totalram;
}
