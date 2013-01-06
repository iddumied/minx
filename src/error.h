#ifndef __MINX_ERROR_H__
#define __MINX_ERROR_H__

#include <stdio.h>

#define FATAL_ERROR         do {                                            \
                                printf("minx: FATAL ERROR. Aborting\n");    \
                                exit(1);                                    \
                            } while (0);

#define FATAL_DESC_ERROR(desc)  do {                                        \
                                    printf( "minx: FATAL ERROR.\n"          \
                                            "%s\n"                          \
                                            "Aborting.\n"                   \
                                            ,desc);                         \
                                    exit(1);                                \
                                } while (0);


#endif // __MINX_ERROR_H__
