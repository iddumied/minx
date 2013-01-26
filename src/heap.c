#include "heap.h"

/*
 * static function prototypes
 */
static HeapNode*    create_new_heapnode             (void);
static uint64_t     get_next_memory_id              (void);

/*
 * static variables
 */
static HeapNode     **heapnodes;
static uint64_t     heapnodes_count;
static uint64_t     memory_id_counter;

/*
 * -----------------------------------------------------------------------------
 *
 *      Heap functions implementations
 *
 * -----------------------------------------------------------------------------
 */

void minx_vpu_heap_setup(void) {
    heapnodes                   = (HeapNode**) malloc( sizeof(*heapnodes) );

    heapnodes[0]->used_state    = HEAPNODE_NOT_ALLOCATED;
    heapnodes[0]->memoryID      = 0;
    heapnodes[0]->size          = 0;
    heapnodes[0]->real_size     = 0;
    heapnodes[0]->memory        = NULL;

    heapnodes_count             = 0;
    memory_id_counter           = 0;
}

void minx_vpu_heap_shutdown(void) {
    uint64_t i;
    for(i = heapnodes_count; i; i--) {
        free(heapnodes[i-1]->memory);
    }
    free(heapnodes);
}

/*
 * -----------------------------------------------------------------------------
 *
 *          Heap functions
 *
 * -----------------------------------------------------------------------------
 */

uint64_t minx_vpu_heap_alloc(uint64_t size) {
    uint64_t i;
    HeapNode *node = NULL;
    for(i = 0; i < heapnodes_count && node == NULL; i++) {
        if(heapnodes[i]->used_state <= HEAPNODE_NOT_USED) {
            node = heapnodes[i];
        }
    }

    /*
     * If no unused/unallocated heapnode was found, create a new one.
     */
    if(node == NULL) {
        node = create_new_heapnode();
        if(node == NULL) {
            goto failed_to_alloc;
        }
    }

    /*
     * node is set here, if it is HEAPNODE_NOT_USED, it has some memory, if it
     * is HEAPNODE_NOT_ALLOCATED it is completely new!
     */

    if(node->used_state == HEAPNODE_NOT_USED) {
        /*
         * Resize it for fitting the required size
         */
        if(size != node->size) {
            minx_vpu_heap_resize(node->memoryID, size);
        }
    }
    else if (node->used_state == HEAPNODE_NOT_ALLOCATED) {
        node->memory = (char*) malloc(size);
        if(node->memory) { /* if malloc worked */
            node->size      = size;
            node->real_size = size;
        }
        else {
            goto failed_to_alloc;
        }

    }

    return node->memoryID;
failed_to_alloc:
    return MINX_VPU_HEAP_UNABLE_TO_ALLOC;
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

/*
 * static function implementations
 */

/*
 * Creates a new heapnode, inserts it in the **heapnodes array, increments
 * heapnodes_count and returns a ptr to the new heapnode
 */
static HeapNode* create_new_heapnode() {
    HeapNode *node = (HeapNode*) malloc( sizeof(*node) );
    heapnodes = realloc(heapnodes, sizeof(*heapnodes) * heapnodes_count+1);
    heapnodes[heapnodes_count] = node;
    heapnodes_count++;

    node->used_state    = HEAPNODE_NOT_ALLOCATED;
    node->memoryID      = get_next_memory_id();
    node->size          = 0;
    node->real_size     = 0;
    node->memory        = NULL;

    return node;
}

static uint64_t get_next_memory_id() {
    return memory_id_counter++;
}
