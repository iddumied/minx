#ifndef __MINX_ERROR_H__
#define __MINX_ERROR_H__

#include <stdlib.h>
#include <stdio.h>

void    minx_error_init                         (void);
void    minx_error_shutdown                     (void);
void    minx_error_global_shutdown              (void);
void    minx_error_register_shutdown_function   (void(*func)(void));

#define F_RUNTIME_ERROR(pref,f,...)                                         \
    do {                                                                    \
        fprintf(stderr,"%s"f,pref,__VA_ARGS__);                             \
    } while (0)

#define FATAL_ERROR FATAL_F_ERROR("func %s\nline %i\n",__func__,__LINE__);

#define FATAL_DESC_ERROR(desc) FATAL_F_ERROR("func %s\nline %i\n%s\n",__func__,__LINE__,desc);

#define FATAL_F_ERROR(f,...)    do {                                        \
                                    printf("minx: FATAL ERROR.\n"           \
                                           f                                \
                                           "\nAborting.\n"                  \
                                           ,__VA_ARGS__);                   \
                                    minx_error_global_shutdown();           \
                                    exit(1);                                \
                                } while(0)

typedef void(*ShutdownFunctionPtr)(void);

#endif // __MINX_ERROR_H__
