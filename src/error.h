#ifndef __MINX_ERROR_H__
#define __MINX_ERROR_H__

#include <stdio.h>

#define FATAL_ERROR         do {                                            \
                                printf( "minx: FATAL ERROR.\n"              \
                                        "func %s\nline %s\n"                \
                                        "Aborting\n", __func__,__LINE__);   \
                                exit(1);                                    \
                            } while (0);

#define FATAL_DESC_ERROR(desc)  do {                                        \
                                    printf( "minx: FATAL ERROR.\n"          \
                                            "func %s\nline %s\n"            \
                                            "%s\n"                          \
                                            "Aborting.\n"                   \
                                            ,__func__,__LINE__,desc);       \
                                    exit(1);                                \
                                } while (0);


#endif // __MINX_ERROR_H__
