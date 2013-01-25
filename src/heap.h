#ifndef __minx_vpu_VPU_HEAP_H__
#define __minx_vpu_VPU_HEAP_H__

#include <stdlib.h>
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
    /*
     * For notes about the 'used_state' variable, have a look at the notes right
     * below the HEAPNODE_USED macro!
     */
    uint8_t     used_state;

    /*
     * The uniq ID, the binary gets for dealing around with it.
     * This ID is a simple counter in the first implementation.
     *
     * TODO:
     * Because a simple counter is not very secure, it should be possible to 
     * configure the VPU to use a randomized value here. The map were this
     * HeapNodes are saved, has to be ordered! Remember that!
     */
    uint64_t    memoryID;

    /*
     * This is the size, the memory has for the binary (in bytes). If the memory 
     * gets resized, the does some optimisation here.
     * If the size gets decremented, just set this variable to the new value. If
     * the sizes gets incremented, do it.
     */
    uint64_t    size;

    /*
     * The real size of the memory in bytes
     */
    uint64_t    real_size;

    /*
     * The memory, in bytes.
     */
    char        *memory;

    /*
     * I named it HeapNode, don't know why. 
     */
} HeapNode;

/*
 * All values != HEAPNODE_USED are HEAPNODE_NOT_USED. 
 *
 * Current implementation does not provide "Garbage Collection" at all. But
 * later, it should be possible to configure the VPU to remove not used memory
 * automatically based on some statistics or so.
 */
enum {
    HEAPNODE_NOT_ALLOCATED = 0,
    HEAPNODE_NOT_USED,
    HEAPNODE_USED,
}

/*
 * setup function for heap.
 *
 * This function gets called before the binary is loaded or run, but after the
 * config is parsed.
 * So the heap can use config to configure itself.
 */
void        minx_vpu_heap_setup     (void);

/*
 * shutdown function for heap.
 *
 * This function gets called if the binary was interpreted successfully and
 * completely or if SIGINT was send.
 */
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
