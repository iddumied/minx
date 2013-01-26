#include "heap.h"

/*
 * static function prototypes
 */
static HeapNode*    create_new_heapnode             (void);
static uint64_t     get_next_memory_id              (void);
static HeapNode*    find_heap                       (uint64_t heap);

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
    if(node == NULL && !(node = create_new_heapnode())) {
        goto failed_to_alloc;
    }

    /*
     * node is set here, if it is HEAPNODE_NOT_USED, it has some memory, if it
     * is HEAPNODE_NOT_ALLOCATED it is completely new!
     */

    /*
     * Resize it for fitting the required size if node was already used
     */
    if(node->used_state == HEAPNODE_NOT_USED && size != node->size) {
        minx_vpu_heap_resize(node->memoryID, size);
    }

    /*
     * Allocate memory for the node if it is new
     */
    else if (node->used_state == HEAPNODE_NOT_ALLOCATED) {
        node->memory = (char*) malloc(size);

        /*
         * if malloc() worked, set the size in the node,
         * else goto failed_to_alloc
         */
        if(node->memory) {
            node->size  = node->real_size = size;
        }
        else {
            goto failed_to_alloc;
        }

    }

    return node->memoryID;
failed_to_alloc:
    return MINX_VPU_HEAP_ERROR;
}

int minx_vpu_heap_resize(uint64_t heap, uint64_t new_size) {
    HeapNode *h = find_heap(heap);

    /*
     * pointer to backup the memory if the realloc fails
     */
    char *backup_memory;
    
    /*
     * return if heap is not found, the vpu has the problem now!
     */
    if(h == NULL)
        goto err;

    /*
     * if the size is already set, everything is okay. If the new_size is equal
     * to the real size (but maybe not to the set size) just set the size
     * variable of the mem.
     */
    if(h->size == new_size || h->real_size == new_size) {
        /*
         * No effect on h->size == new_size but on h->real_size == new_size
         */
        h->size = new_size;

        /*
         * nothing to do!
         */
        goto no_err;
    }

    /*
     * if the size is decreased, just set the size variable to the smaller value
     * and don't do any realloc() call, for the speed!
     * 
     * OR
     *
     * If the new_size is greater than the actual size, but smaller than the
     * real size, just set the size variable, for the speed!
     */
    if( (h->size > new_size) || 
        (h->size < new_size && h->real_size > new_size) ) {

        h->size = new_size;
        goto no_err;
    }
    
    /*
     * And finally, if the new_size is greater than the real size, do the realloc.
     */
    else {

        /*
         * If the memory realloc fails, we have our pointer to the memory
         * backuped and can reset it.
         *
         * So if realloc fails, we still have the memory and it causes no memory
         * leak here.
         */
        backup_memory = h->memory;
        h->memory = (char*) realloc(h->memory, new_size);
        if(h->memory == NULL) {
            h->memory = backup_memory;
            goto err;
        }

        /*
         * If the realloc() worked, set the new size.
         */
        h->real_size = h->size = new_size;
    }

no_err:
    return MINX_VPU_HEAP_OK;
err:
    return MINX_VPU_HEAP_ERROR;
}

uint64_t minx_vpu_heap_get_size(uint64_t heap) {
    HeapNode *h = find_heap(heap);
    if(h == NULL)
        goto err;
    return h->size;

    /*
     * On error, do not return MINX_VPU_HEAP_ERROR, because if this value
     * changes in future and no one cares about this function, it could cause
     * weird errors! 
     *
     * If the get_size failes because the heapnode does not exist, return zero.
     * This is true if the heapnode does not exist!
     */
err:
    return 0;
}

void minx_vpu_heap_put(uint64_t heap, uint64_t offset, uint64_t val, unsigned int bytes) {
}

void minx_vpu_heap_read(uint64_t heap, uint64_t offset, unsigned int bytes, uint64_t *dest) {
}

/*
 * The minx_vpu_heap_free() function does NOT remove the HeapNode from the
 * memory if it gets called. It just marks it as not used, so for later use,
 * this heapnode can be used again.
 *
 * But it sets the memory to 0x00!
 */
void minx_vpu_heap_free(uint64_t heap) {
    HeapNode *h = find_heap(heap);
    if(h == NULL)
        goto ready;

    h->used_state   = HEAPNODE_NOT_USED;
    h->memory       = memset(h->memory, 0x00, h->real_size);

ready:
    return;
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

/*
 * Currently, the find_heap() function does a linear search. Lateron, when this
 * stuff gets optimized (sorted), we should do a binary search!
 */
static HeapNode* find_heap(uint64_t heapID) {
    uint64_t i;
    HeapNode *found = NULL ;
    for(i = 0; i < heapnodes_count && found == NULL; i++) {
        if(heapnodes[i]->memoryID == heapID) {
            found = heapnodes[i];
        }
    }
    return found;
}
