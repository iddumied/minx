#ifndef __MINX_ERROR_H__
#define __MINX_ERROR_H__

#include <stdlib.h>
#include <stdio.h>


void    minx_error_init                         (void);
void    minx_error_shutdown                     (void);
void    minx_error_global_shutdown              (void);
void    minx_error_register_shutdown_function   (void(*func)(void));


#define FATAL_ERROR         do {                                            \
                                printf( "minx: FATAL ERROR.\n"              \
                                        "func %s\nline %i\n"                \
                                        "Aborting\n", __func__,__LINE__);   \
                                exit(1);                                    \
                            } while (0);

#define FATAL_DESC_ERROR(desc)  do {                                        \
                                    printf( "minx: FATAL ERROR.\n"          \
                                            "func %s\nline %i\n"            \
                                            "%s\n"                          \
                                            "Aborting.\n"                   \
                                            ,__func__,__LINE__,desc);       \
                                    exit(1);                                \
                                } while (0);


#endif // __MINX_ERROR_H__
