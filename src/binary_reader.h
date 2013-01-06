#ifndef __MINX_BINARY_READER_H__
#define __MINX_BINARY_READER_H__

#include <stdint.h>

uint16_t    minx_binary_get_at(uint64_t p, unsigned int number_of_bytes);
signed int  minx_binary_exists_at(uint64_t p);

#endif // __MINX_BINARY_READER_H__
