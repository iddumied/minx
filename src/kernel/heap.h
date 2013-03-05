#ifndef __MINX_KERNEL_HEAP_H__
#define __MINX_KERNEL_HEAP_H__

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifdef DEBUGGING
#include <inttypes.h>
#include <stdio.h>
#endif //DEBUGGING

#include "util/error.h"
#include "util/config.h"

/*
 * if --fast is set:
 *
 * The heap module does allocate more heapnodes than it requires. 
 * Until HEAPNODES_DOUBLE_REALLOC_LIMIT heapnode-ptrs are allocated, it allocates
 * double mem.
 * Until HEAPNODES_FAST_REALLOC_LIMIT it allocates HEAPNODES_REALLOC_FAST_STEP.
 * Else it allocs HEAPNODES_REALLOC_STEP
 *
 */
#define HEAPNODES_DOUBLE_REALLOC_LIMIT      500
#define HEAPNODES_FAST_REALLOC_LIMIT        1000
#define HEAPNODES_REALLOC_FAST_STEP         50
#define HEAPNODES_REALLOC_STEP              5


/** @typedef HeapNode
 *
 * @brief A HeapNode type with all information about one Heap
 *
 * used: if this heapnode is used, this value is UINT8_MAX
 * when it is unused it is less than UINT8_MAX, and gets decremented each
 * kernel_MEM_CLEANUP times. If it is zero, it is removed from memory (free()).
 *
 * first_byte_addr: address of the first byte of this set of memory. Used for
 * the program.
 *
 * size: size of the memory. Used for both internals and the program.
 * *memory: pointer to the memory.
 *
 * I named it HeapNode, don't know why. 
 */
typedef struct {
    /**
     * @brief State of this HeapNode
     *
     * For notes about the 'used_state' variable, have a look at the notes right
     * below the HEAPNODE_USED macro!
     */
    uint8_t     used_state;

    /**
     * @brief ID of this HeapNode (uniq)
     *
     * The uniq ID, the binary gets for dealing around with it.
     * This ID is a simple counter in the first implementation.
     *
     * TODO:
     * Because a simple counter is not very secure, it should be possible to 
     * configure the kernel to use a randomized value here. The map were this
     * HeapNodes are saved, has to be ordered! Remember that!
     */
    uint64_t    memoryID;

    /**
     * @brief The size (for the running program) of the HeapNode
     *
     * This is the size, the memory has for the binary (in bytes). If the memory 
     * gets resized, the does some optimisation here.
     * If the size gets decremented, just set this variable to the new value. If
     * the sizes gets incremented, do it.
     */
    uint64_t    size;

    /**
     * @brief The real size of the memory in bytes
     */
    uint64_t    real_size;

    /**
     * @brief The memory. In Bytes.
     */
    char        *memory;

} HeapNode;

/**
 * @brief Status enumerator for HeapNode type
 *
 * All values != HEAPNODE_USED are HEAPNODE_NOT_USED. 
 *
 * Current implementation does not provide "Garbage Collection" at all. But
 * later, it should be possible to configure the kernel to remove not used memory
 * automatically based on some statistics or so.
 */
enum {
    HEAPNODE_NOT_ALLOCATED = 0,
    HEAPNODE_NOT_USED,
    HEAPNODE_USED,
};

/*
 * If the anything failed, 0x00 should be returned, else 1
 */
#define MINX_KERNEL_HEAP_OK                    ((uint64_t)0x01)
#define MINX_KERNEL_HEAP_ERROR                 ((uint64_t)0x00)

/*
 * setup function for heap.
 *
 * This function gets called before the binary is loaded or run, but after the
 * config is parsed.
 * So the heap can use config to configure itself.
 */
void        minx_kernel_heap_init               (void);

/*
 * shutdown function for heap.
 *
 * This function gets called if the binary was interpreted successfully and
 * completely or if SIGINT was send.
 */
void        minx_kernel_heap_shutdown           (void);

uint64_t    minx_kernel_heap_alloc              (uint64_t size );
int         minx_kernel_heap_resize             (uint64_t heap, uint64_t new_size);
uint64_t    minx_kernel_heap_get_size           (uint64_t heap);

int         minx_kernel_heap_put                (uint64_t heap, 
                                                uint64_t offset, 
                                                unsigned int bytes,
                                                uint64_t val);

int         minx_kernel_heap_read               (uint64_t heap, 
                                                uint64_t offset, 
                                                unsigned int bytes, 
                                                uint64_t *dest);

int         minx_kernel_heap_free               (uint64_t heap);

#ifdef DEBUGGING
void        minx_kernel_heap_print_heapnode     (uint64_t heap);
void        minx_kernel_heap_print_heap         ();
#endif // DEBUGGING


#endif // __MINX_KERNEL_HEAP_H__
