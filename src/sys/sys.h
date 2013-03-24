#ifndef __MINX_SYS_H__
#define __MINX_SYS_H__

#ifdef __linux__
#include <sys/sysinfo.h>
#else //_WIN32 || _WIN64
#error "Can't be compiled on non-linux OSes yet"
#endif 

#include <stdlib.h>

#include "util/error.h"
/*
 *
 *
 * System interface functions
 *
 *
 */

void        minx_sys_init               (void);
void        minx_sys_shutdown           (void);

long        minx_sys_get_ramsize        (void);


#endif //__MINX_SYS_H__
