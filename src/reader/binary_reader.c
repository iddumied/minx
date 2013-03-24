#include "reader/binary_reader.h"

/*
 * The binary reader works after a simple algo:
 *
 * The operations on each node are counted. After n steps, the nodes witch were
 * accessed less than 25% of the stepcount are free'd.
 *
 * This value can be changed! It's just a testing value
 * It's defined in def/sizes.h
 */

/**
 * @brief Enum to set the chunk state
 */
enum cchunkstate {
    FREE,
    ALLOCATED,
};

/**
 * @brief A chunk to store a part of the binary.
 */
struct cchunk {
    enum cchunkstate    state; /*!< inidicates if this chunk is currently available */
    size_t              size; /*!< size of this chunk (its data) */
    unsigned int        num; /*!< chunk-number. Used to calculate addresses, so starting with zero */
    uint64_t            hit_counter; /*!< Counts the hits in one gc-iteration */
    char                *data; /*!< the binary. */
};

/*
 * --------------------------
 * static function prototypes
 * --------------------------
 */

static long             get_file_size                   (FILE *f);
static long             calculate_caching_size          (long filesize);
static struct cchunk*   get_new_cchunk                  (void);
static void             uncache                         (struct cchunk*);
static unsigned int     calculate_chunknum_for_address  (uint64_t addr);
static void             loadchunk                       (struct cchunk *chunk);
static void             gc                              (void);

/*
 * static variables
 */

static FILE             *file;
static long             filesize;
static long             cachechunk_size;
static struct cchunk    **chunks;
static uint64_t         chunkcount;

/**
 * @brief When this counter hits a specific value, the cchunks are cleaned up.
 */
static uint64_t         gc_iteration_counter;

/*
 *
 * ------------------------
 * Function implementations
 * ------------------------
 *
 */

/**
 * @brief Initialize the binary reader
 *
 * @param f The file, containing the binary
 */
void minx_binary_init(FILE *f) {
    file = f;
    filesize                = get_file_size(f);
    cachechunk_size         = calculate_caching_size(filesize);
    gc_iteration_counter    = 0;

    /*
     * initialize the chunks array.
     *
     */
    uint64_t cache_created;

    /* calc the number, how many chunks are required */
    for(    chunkcount = 0, cache_created = 0;
            cache_created < filesize; 
            chunkcount++, cache_created += cachechunk_size);

    chunks = (struct cchunk**) malloc(sizeof(struct cchunk*) * chunkcount+1);

    /* allocate the chunks, but not the binary */
    for(    chunkcount = 0, cache_created = 0;
            cache_created < filesize; 
            chunkcount++, cache_created += cachechunk_size) {

        chunks[chunkcount] = get_new_cchunk();
    }

    loadchunk(chunks[0]); /*!< precache the first chunk */

#ifdef DEBUGGING
    minxbinarydbgprintf("Binary initialized with %"PRIu64" chunks, %ld Bytes max per chunk", 
            chunkcount, cachechunk_size);
#endif

}

/**
 * @brief shutdown the binary reader
 *
 * Free all binary chunks
 */
void minx_binary_shutdown(void) {
#ifdef DEBUGGING
    minxbinarydbgprint("Binary shutdown");
#endif

    for(;chunkcount != 0; chunkcount--) {
        free(chunks[chunkcount-1]->data);
        chunks[chunkcount-1]->data = NULL;
    }
    free(chunks);
}

/**
 * @brief get bytes from the binary at a specific position
 *
 * @return a pointer to the bytes
 */
void * minx_binary_get_at(  uint64_t p, 
                            unsigned int number_of_bytes, 
                            void *dest, 
                            size_t destsize) {
#ifdef DEBUGGING
    minxbinarydbgprintf("Get Binary: %u Bytes", number_of_bytes);
#endif

    unsigned int chunknum           = calculate_chunknum_for_address(p);
    uint64_t first_addr_in_chunk    = cachechunk_size * (chunknum);
    unsigned int chunk_byte_addr    = p - first_addr_in_chunk;
    int is_multichunk               = (chunk_byte_addr + number_of_bytes) > cachechunk_size;

    memset(dest, 0x00, destsize);
    loadchunk(chunks[chunknum]);
    if(is_multichunk) {
        if(chunknum+1 > chunkcount) {
            FATAL_F_ERROR(
                    "Cannot load binary chunk %u, does not exist!", 
                    chunknum+1);
        }

        loadchunk(chunks[chunknum+1]);

        unsigned int chunk_1_copy_size = cachechunk_size - chunk_byte_addr;
        unsigned int chunk_2_copy_size = number_of_bytes - chunk_1_copy_size;

        memcpy(dest, &(chunks[chunknum]->data[chunk_byte_addr]), chunk_1_copy_size);
        memcpy(((char*)dest)+chunk_1_copy_size, chunks[chunknum+1]->data, chunk_2_copy_size);

        chunks[chunknum+1]->hit_counter++;
    }
    else {
        memcpy(dest, &(chunks[chunknum]->data[chunk_byte_addr]), number_of_bytes);
    }

    chunks[chunknum]->hit_counter++;

    gc_iteration_counter++;
    if(gc_iteration_counter > ITERATIONS_UNTIL_GC) {
        gc();
        gc_iteration_counter = 0;
    }

    return dest;
}

/**
 * @brief Checks if the binary exists at a given position
 *
 * @param p The position to check
 *
 * @return true if the binary exists at the position pointed to by p, else false.
 */
signed int minx_binary_exists_at(uint64_t p) {
    return filesize > p;
}

#ifdef DEBUGGING
/**
 * @brief Print the binary
 */
void minx_binary_print(void) {
    minxbinarydbgprint("Print binary");

    struct cchunk   *chunk;
    long            i;
    unsigned int    line = 0;
    
    printf("0x00000000 : ");

    for(chunk = chunks[0]; chunk != chunks[chunkcount-1]; chunk++) {
        for(i = 0 ; i < cachechunk_size; i++ ) {
            if( chunk->data[i] == 0 ) {
                printf("0x00000000 ");
            }
            else {
                printf("%#010x ", chunk->data[i]);
            }
            if((i+1) % 8 == 0) {
                printf("\n%#010x : ", ++line);
            }
        } 
    }
    printf("\n");
}
#endif

/*
 * -------------------------------
 * static function implementations
 * -------------------------------
 */

/**
 * @brief Get the size of the passed File
 *
 * @param f The file to get the size from
 *
 * @return The size of the file
 */
static long get_file_size(FILE *f) {
    long res;
    fseek(f, 0L, SEEK_END);
    res = ftell(f);
    fseek(f, 0L, SEEK_SET);
    return res;
}

/**
 * @brief Calculate the maximum size of one cache chunk.
 *
 * @param filesize The size of the File
 *
 * If __MINIRAM__ is defined, the size of a chunk is taken from the 
 * MINX_ONE_CHUNK_LOAD_SIZE macro, so it is defined at compile time.
 *
 * Else, it is calculated by the MINX_BINARY_MAX_RAM_PERCENTAGE macro.
 *
 * @return The max-size of a cache-chunk
 */
static long calculate_caching_size(long filesize) {
#ifdef DEBUGGING
    minxbinarydbgprint("Calculate caching size");
#endif

#ifdef __MINIRAM__
    return MINX_ONE_CHUNK_LOAD_SIZE;
#else //__MINIRAM__

    long ram_size   = minx_sys_get_ramsize();

    /**
     * @brief In want of a better variable name.
     */
    long useabe_ram = ram_size / 100 * MINX_BINARY_MAX_RAM_PERCENTAGE;

    return useabe_ram / MINX_BINARY_MIN_CHUNK_COUNT;
#endif //__MINIRAM__
}

/**
 * @brief Allocate a new struct cchunk
 *
 * @return pointer to new struct cchunk or NULL on failure
 */
static struct cchunk* get_new_cchunk(void) {
    struct cchunk *c = (struct cchunk*) malloc(sizeof(struct cchunk));
    
    if(c) {
        c->state    = FREE;
        c->size     = 0;
        c->num      = 0;
        c->hit_counter = 0;
        c->data     = NULL;
    }

    return c;
}

/**
 * @brief Free the memory of this struct cchunk, set its state to FREE
 *
 * @param c A pointer to the struct cchunk to free
 */
static void uncache(struct cchunk *c) {
#ifdef DEBUGGING
    minxbinarydbgprintf("uncache chunk %u", c->num);
#endif

    c->state = FREE;
    c->hit_counter = 0;
    free(c->data);
}

/**
 * @brief Calculate the chunk number (index in chunks array) by the passed address
 *
 * @param addr The address to calculate the chunk number for
 *
 * @return The number of the chunk where the address points into
 */
static unsigned int calculate_chunknum_for_address(uint64_t addr) {
#ifdef DEBUGGING
    minxbinarydbgprint("Calculate chunknum");
#endif
    
    return (addr - (addr % cachechunk_size)) / cachechunk_size;
}

/**
 * @brief Load a chunk. If the chunk is not allocated, load its data, else return
 *
 * @param chunk The chunk to load
 */
static void loadchunk(struct cchunk *chunk) {
    if(chunk->state == ALLOCATED)
        return;

#ifdef DEBUGGING
    minxbinarydbgprintf("Load chunk %u", chunk->num);
#endif

    chunk->data = (char*) malloc(cachechunk_size);

    fseek(file, 0L, (cachechunk_size * chunk->num));
    memset(chunk->data, 0x00, cachechunk_size);
    fread(chunk->data, sizeof(char), cachechunk_size, file);

    /*
     * Finally, set the chunk status
     */
    chunk->state = ALLOCATED;
}

/**
 * @brief Garbage collector like mechanizm to remove old chunks of binary.
 */
static void gc(void) {
    unsigned int i, j = 0;
    for(i = 0; i < chunkcount; i++) {
#ifdef DEBUGGING
            printf("Uncache %u?\n", i);
            fflush(stdout);
#endif
        if(chunks[i]->hit_counter < (ITERATIONS_UNTIL_GC / 100 * BINARY_FREE_PERCENTAGE)) {
            uncache(chunks[i]);
            j++;
        }
    }
#ifdef DEBUG
    minxbinarydbgprintf("Called garbage collector, free'd %u chunks", i);
#endif
}
