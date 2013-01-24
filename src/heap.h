#ifndef __minx_vpu_VPU_HEAP_H__
#define __minx_vpu_VPU_HEAP_H__

#include <stdint.h>

/*
 *
 * Heap 
 *
 * used: if this heapnode is used, this value is UINT8_MAX
 * when it is unused it is less than UINT8_MAX, and gets decremented each
 * VPU_MEM_CLEANUP times. If it is zero, it is removed from memory (free()).
 *
 * first_byte_addr: address of the first byte of this set of memory. Used for
 * the program.
 *
 * size: size of the memory. Used for both internals and the program.
 * *memory: pointer to the memory.
 */
typedef struct {
    uint8_t     used;
    uint64_t    first_byte_addr;
    uint64_t    size;
    uint64_t    real_size;
    char        *memory;
} HeapNode;

#define HEAPNODE_USED           UINT8_MAX
#define HEAPNODE_NOT_USED       0

void        minx_vpu_heap_setup     (void);
void        minx_vpu_heap_shutdown  (void);

uint64_t    minx_vpu_heap_alloc     (uint64_t size );
int         minx_vpu_heap_resize    (uint64_t heap, uint64_t new_size);
uint64_t    minx_vpu_heap_get_size  (uint64_t heap);

void        minx_vpu_heap_put       (   uint64_t heap, 
                                        uint64_t offset, 
                                        uint64_t val, 
                                        unsigned int bytes);

void        minx_vpu_heap_read      (   uint64_t heap, 
                                        uint64_t offset, 
                                        unsigned int bytes, 
                                        uint64_t *dest);

void        minx_vpu_heap_free      (uint64_t heap);


#endif // __minx_vpu_VPU_HEAP_H__
