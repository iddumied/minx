#ifndef __MINX_BINARY_READER_H__
#define __MINX_BINARY_READER_H__

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "error.h"

#if (defined DEBUGGING | defined DEBUG)
#include <stdio.h>
#endif 

#ifdef DEBUG
#include "debug.h"
#endif

void            minx_binary_init        ( FILE *f );
void            minx_binary_shutdown    (void);
void *          minx_binary_get_at      (   uint64_t p, 
                                            unsigned int number_of_bytes, 
                                            void *dest);

signed int      minx_binary_exists_at   (uint64_t p);

#ifdef DEBUGGING
void            minx_binary_print       (void);
#endif //DEBGUGGING

#endif // __MINX_BINARY_READER_H__
