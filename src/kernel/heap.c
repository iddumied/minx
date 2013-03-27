#include "heap.h"

/*
 * static function prototypes
 */
static HeapNode*    create_new_heapnode             (void);
static uint64_t     get_next_memory_id              (void);
static HeapNode*    find_heap                       (uint64_t heap);
static void         check_if_memory_exists          (HeapNode *h, 
                                                    uint64_t from, 
                                                    unsigned int to);

/*
 * static variables
 */
static HeapNode     **heapnodes;
static uint64_t     last_heapnode_ptr;
static uint64_t     heapnodes_ptr_count;
static uint64_t     memory_id_counter;

/*
 * -----------------------------------------------------------------------------
 *
 *      Heap functions implementations
 *
 * -----------------------------------------------------------------------------
 */

/**
 * @brief Initialize function for the Heap
 *
 * Initializes the heap in the kernel. It registers its shutdown function, 
 * allocates memory for the heapnodes array, initializes the first heapnode and
 * initializes the memory_id_counter.
 */
void minx_kernel_heap_init(void) {
    minx_error_register_shutdown_function(minx_kernel_heap_shutdown);

    heapnodes                   = (HeapNode**)  malloc( sizeof(HeapNode*) );
    heapnodes[0]                = (HeapNode*)   malloc( sizeof(HeapNode) );

    heapnodes[0]->used_state    = HEAPNODE_NOT_ALLOCATED;
    heapnodes[0]->memoryID      = 0;
    heapnodes[0]->size          = 0;
    heapnodes[0]->real_size     = 0;
    heapnodes[0]->memory        = NULL;

    last_heapnode_ptr           = 0;
    heapnodes_ptr_count         = 1;

    /* set the memory_id_counter to 1, because zero is returned on error */
    memory_id_counter           = MINX_KERNEL_HEAP_ERROR + 1;

    /*
     * if configured to be fast, allocate HEAPNODES_PREALLOCATE_COUNT heapnodes 
     * at setup.
     */
    if(minx_config_get(CONF_FAST)->b) {
        unsigned int i;
        for(i = 0; i < HEAPNODES_PREALLOCATE_COUNT; i++) {
            create_new_heapnode();
        }
    }
}

/**
 * @brief Shutdown the heap
 *
 * Frees all heapnodes
 */
void minx_kernel_heap_shutdown(void) {
    uint64_t i;
    for(i = last_heapnode_ptr; i; i--) {
        free(heapnodes[i-1]);
    }
}

/*
 * -----------------------------------------------------------------------------
 *
 *          Heap functions
 *
 * -----------------------------------------------------------------------------
 */

/**
 * @brief Allocate a heapnode with a given size
 *
 * If there are unused heapnodes, take one of them, to keep memuse as smart as
 * possible.
 *
 * @param size The size, the heapnode should have
 *
 * @return the memoryID of the heapnode
 */
uint64_t minx_kernel_heap_alloc(uint64_t size) {
    if( size == 0 ) {
        FATAL_DESC_ERROR("cannot alloc heap with size 0 (zero)");
    }

    /*
     * try to find unused heapnode
     */
    uint64_t i;
    HeapNode *node = NULL;
    for(i = 0; i < last_heapnode_ptr && node == NULL; i++) {
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
        minx_kernel_heap_resize(node->memoryID, size);
    }

    /*
     * Allocate memory for the node if it is new
     */
    else /*if (node->used_state == HEAPNODE_NOT_ALLOCATED)*/ {
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

    /*
     * Mark heapnode as used when it is allocated.
     * if it gets free'd, it's not used
     */
    node->used_state = HEAPNODE_USED;

    /*
     * return the memoryID of the node. The real memory is never passed to the
     * kernel!
     */
    return node->memoryID;
failed_to_alloc:
    return MINX_KERNEL_HEAP_ERROR;
}

/**
 * @brief Resize a Heapnode
 *
 * If the node is bigger as the next size, only set a parameter in the heapnode
 * structure to avoid often calls to realloc(). If it is bigger, do the call.
 *
 * @param heap The ID of the Heapnode to resize
 * @param new_size The size, it has to have after the resize
 *
 * @return A status if the resize worked or not
 */
int minx_kernel_heap_resize(uint64_t heap, uint64_t new_size) {
    HeapNode *h = find_heap(heap);

    /*
     * pointer to backup the memory if the realloc fails
     */
    char *backup_memory;
    
    /*
     * return if heap is not found, the kernel has the problem now!
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
    return MINX_KERNEL_HEAP_OK;
err:
    return MINX_KERNEL_HEAP_ERROR;
}

/**
 * @brief Get size of a heapnode
 *
 * @param heap The ID of the heapnode to get the size from
 *
 * @return The size of the Heapnode
 */
uint64_t minx_kernel_heap_get_size(uint64_t heap) {
    HeapNode *h = find_heap(heap);

    /*
     * On error, do not return MINX_KERNEL_HEAP_ERROR, because if this value
     * changes in future and no one cares about this function, it could cause
     * weird errors! 
     *
     * If the get_size failes because the heapnode does not exist, return zero.
     * This is true if the heapnode does not exist!
     */
    return h == NULL ? 0 : h->size;
}

/**
 * @brief Put something in the heapnode
 *
 * @param heap The ID of the heapnode to put something in
 * @param offset The byte where to put the values
 * @param bytes The number of bytes, how much to put there
 * @param val The value
 *
 * @return A Status if the put worked or not
 */
int minx_kernel_heap_put(uint64_t heap, uint64_t offset, unsigned int bytes, uint64_t val) {
    HeapNode *h = find_heap(heap);

    if(h == NULL)
        goto heap_not_found;

    check_if_memory_exists(h, offset, bytes);

    memcpy(&h->memory[offset], &val, bytes); 
    h->used_state = HEAPNODE_USED;

    return MINX_KERNEL_HEAP_OK;

heap_not_found:
    return MINX_KERNEL_HEAP_ERROR;
}

int minx_kernel_heap_read(uint64_t heap, uint64_t offset, unsigned int bytes, uint64_t *dest) {
    HeapNode *h = find_heap(heap);

    if(h == NULL)
        goto err;
    
    check_if_memory_exists(h, offset, bytes);

    memcpy(dest, &h->memory[offset], bytes);

    return MINX_KERNEL_HEAP_OK;

err:
    return MINX_KERNEL_HEAP_ERROR;
}

/**
 * @brief Get a HeapNode* to heap identified by ID
 *
 * Externally visible alias for find_heap();
 *
 * @param heap The heap-ID
 *
 * @return The Heap requested by ID
 */
HeapNode * minx_kernel_heap_get(uint64_t heap) {
    return find_heap(heap);
}

/*
 * @brief Remove a Heapnode from memory (seemingly)
 *
 * The minx_kernel_heap_free() function does NOT remove the HeapNode from the
 * memory if it gets called. It just marks it as not used, so for later use,
 * this heapnode can be used again.
 *
 * But it sets the memory to 0x00!
 *
 * @param heap The ID of the Heapnode to remove
 *
 * @return A status if the free worked or not
 */
int minx_kernel_heap_free(uint64_t heap) {
    HeapNode *h = find_heap(heap);
    if(h == NULL) {
        FATAL_F_ERROR("Heapnode not found: %"PRIu64, heap);
    }
    if(h->used_state < HEAPNODE_USED) {
        FATAL_F_ERROR("Heapnode not in use here: %"PRIu64, heap);
    }

    h->used_state   = HEAPNODE_NOT_USED;
    h->memory       = memset(h->memory, 0x00, h->real_size);

    return MINX_KERNEL_HEAP_OK;
}

#ifdef DEBUGGING
/**
 * @brief Print a Heapnode
 *
 * @param heap The ID of the Heapnode to print
 */
void minx_kernel_heap_print_heapnode(uint64_t heap) {
    HeapNode *h = find_heap(heap);

    if(h == NULL)
        goto ready;
    
    uint64_t i;
    unsigned int line = 0;
    printf("0x00000000 : ");
    for( i = 0 ; i < h->size; i++ ) {
        if( h->memory[i] == 0 ) {
            printf("0x00 ");
        }
        else {
            printf("%#02x ", h->memory[i]);
        }

        if((i+1) % 8 == 0) {
            printf("\n%#010x : ", ++line);
        }
    }
    printf("\n");


ready:
    return;
}
#endif //DEBUGGING

#ifdef DEBUGGING
void minx_kernel_heap_print_heap() {
    uint64_t i;
    unsigned int line = 0, j = 0;
    for(i = 0 ; i < last_heapnode_ptr; i++) {
        if(minx_config_get(CONF_HEX)->b) {
            printf("ID: %#010"PRIx64"\n", heapnodes[i]->memoryID);
        }
        else {
            printf("ID: %"PRIu64"\n", heapnodes[i]->memoryID);
        }

        printf("0x00000000 : ");
        for(j = 0; j < heapnodes[i]->size; j++ ) {
            if( heapnodes[i]->memory[j] == 0 ) {
                printf("0x00 ");
            }
            else {
                printf("%#02x ", heapnodes[i]->memory[j]);
            }

            if((j+1) % 8 == 0) {
                printf("\n%#010x : ", ++line);
            }
        }
        printf("\n");
    }
    printf("\n");
}
#endif //DEBUGGING

/*
 * static function implementations
 */

/**
 * @brief Create a new Heapnode
 *
 * Creates a new heapnode, inserts it in the **heapnodes array, increments
 * last_heapnode_ptr and returns a ptr to the new heapnode
 *
 * realloc algorythm:
 *
 * The heap module does allocate more heapnodes than it requires. 
 * Until HEAPNODES_DOUBLE_REALLOC_LIMIT heapnode-ptrs are allocated, it allocates
 * double mem.
 * Until HEAPNODES_FAST_REALLOC_LIMIT it allocates HEAPNODES_REALLOC_FAST_STEP.
 * Else it allocs HEAPNODES_REALLOC_STEP
 *
 * Note: This is just pointer reallocating!
 *
 * @return A pointer to the new Heapnode
 */
static HeapNode* create_new_heapnode() {

    /*
     * realloc logic
     */
    if( minx_config_get(CONF_FAST)->b && 
        heapnodes_ptr_count < HEAPNODES_DOUBLE_REALLOC_LIMIT ) {

        heapnodes_ptr_count = heapnodes_ptr_count * 2;
    }
    else if(minx_config_get(CONF_FAST)->b && 
            heapnodes_ptr_count > HEAPNODES_DOUBLE_REALLOC_LIMIT &&
            heapnodes_ptr_count < HEAPNODES_FAST_REALLOC_LIMIT) {
        heapnodes_ptr_count += HEAPNODES_REALLOC_FAST_STEP;
    }
    else {
        heapnodes_ptr_count += HEAPNODES_REALLOC_STEP;
    }

    /*
     * realloc call
     */
    heapnodes = (HeapNode**) realloc(heapnodes, sizeof(HeapNode*) * heapnodes_ptr_count);

    /*
     * initialize heapnodes
     */
    uint64_t i;
    for(i = last_heapnode_ptr; i < (heapnodes_ptr_count-last_heapnode_ptr); i++) {
        heapnodes[i] = (HeapNode*) malloc(sizeof(HeapNode));

        heapnodes[i]->used_state    = HEAPNODE_NOT_ALLOCATED;
        heapnodes[i]->memoryID      = get_next_memory_id();
        heapnodes[i]->size          = 0;
        heapnodes[i]->real_size     = 0;
        heapnodes[i]->memory        = NULL;
    }

    /* 
     * return pointer to first new heapnode
     */
    last_heapnode_ptr++;
    return heapnodes[last_heapnode_ptr-1];
}

/**
 * @brief Get the Next memory ID
 *
 * @return the next memory ID
 */
static uint64_t get_next_memory_id() {
    return memory_id_counter++;
}

/**
 * @brief Find a Heapnode by ID
 *
 * Currently, the find_heap() function does a linear search. Later on, when this
 * stuff gets optimized (sorted), we should do a binary search!
 *
 * This function exits the VPU if the heapnode could not be found
 *
 * @param heapID The ID of the heapnode to find
 *
 * @return The found heapnode
 */
static HeapNode* find_heap(uint64_t heapID) {
    uint64_t i;
    HeapNode *found = NULL ;
    for(i = 0; i < last_heapnode_ptr && found == NULL; i++) {
        if(heapnodes[i]->memoryID == heapID) {
            found = heapnodes[i];
        }
    }

    if(found == NULL) {
        FATAL_F_ERROR("Cannot find memory with ID %"PRIu64"!", heapID);
    }

    return found;
}

/**
 * @brief Check, if a range of memory in a heapnode exists
 *
 * Do some security stuff here:
 *
 * - check if the memory exists at 'offset'
 *
 * - calculate the last address where data is put. If this address exists,
 *   everything is okay, else fail hard.
 *
 * @param h the HeapNode to check 
 * @param from first address (offset)
 * @param to how many bytes to check, this can maximal be 8, so no problem with
 * uint
 *
 * This function exits the VPU if there is a error!
 *
 * @param h The heapnode to check
 * @param from The first byte which should be checked
 * @param to The last byte which should be checked
 */
static void check_if_memory_exists(HeapNode *h, uint64_t from, unsigned int to) {
    if(h->size < from)
        FATAL_DESC_ERROR("Cannot acces memory at unallocated address");
    uint64_t last_put_addr = from + to;
    if(h->size < last_put_addr)
        FATAL_DESC_ERROR("Not enought memory to write to");
}
