#ifndef __MINX_BINARY_READER_H__
#define __MINX_BINARY_READER_H__

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "error.h"

#ifdef DEBUG
#include <stdio.h>
#include "debug.h"
#endif

void            minx_binary_init        ( FILE *f );
void            minx_binary_shutdown    (void);
void *          minx_binary_get_at      (   uint64_t p, 
                                            unsigned int number_of_bytes, 
                                            void *dest);

uint16_t *      minx_get_opcode         (uint64_t p);
signed int      minx_binary_exists_at   (uint64_t p);

#endif // __MINX_BINARY_READER_H__
