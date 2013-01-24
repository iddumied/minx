#ifndef __MINX_VPU_HEAP_H__
#define __MINX_VPU_HEAP_H__

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

#endif // __MINX_VPU_HEAP_H__
