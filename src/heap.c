#include "heap.h"

/*
 * static variables
 */
static HeapNode     *heapnodes;


/*
 * -----------------------------------------------------------------------------
 *
 *      Heap functions implementations
 *
 * -----------------------------------------------------------------------------
 */

void minx_vpu_heap_setup(void) {
}

void minx_vpu_heap_shutdown(void) {
}

/*
 * -----------------------------------------------------------------------------
 *
 *          Heap functions
 *
 * -----------------------------------------------------------------------------
 */

uint64_t minx_vpu_heap_alloc(uint64_t size) {
}

int minx_vpu_heap_resize(uint64_t heap, uint64_t new_size) {
}

uint64_t minx_vpu_heap_get_size(uint64_t heap) {
}

void minx_vpu_heap_put(uint64_t heap, uint64_t offset, uint64_t val, unsigned int bytes) {
}

void minx_vpu_heap_read(uint64_t heap, uint64_t offset, unsigned int bytes, uint64_t *dest) {
}

void minx_vpu_heap_free(uint64_t heap) {
}
