#include "reader/binary_reader.h"

enum cchunkstate {
    FREE,
    ALLOCATED,
};

struct cchunk {
    enum cchunkstate    state; /*!< inidicates if this chunk is currently available */
    size_t              size; /*!< size of this chunk (its data) */
    unsigned int        num; /*!< chunk-number. Used to calculate addresses, so starting with zero */
    char                *data; /*!< the binary. */
};

/*
 * --------------------------
 * static function prototypes
 * --------------------------
 */

static long             get_file_size               (FILE *f);
static long             calculate_caching_size      (long filesize);
static void             precache                    (void);
static struct cchunk*   get_new_cchunk              (void);
static void             uncache                     (struct cchunk*);

/*
 * static variables
 */

static long             filesize;
static long             cachechunk_size;
static struct cchunk    *chunks;

/**
 * @brief Initialize the binary reader
 *
 * @param f The file, containing the binary
 */
void minx_binary_init(FILE *f) {
    filesize        = get_file_size(f);
    cachechunk_size = calculate_caching_size(filesize);
    precache();
}

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
 * Sizes are calculated by a simple algorithm. If the size of the binary is less
 * than n bytes, it will be loaded into RAM completely (well, if your OS does
 * swapping, I don't care). 
 *
 * n is defined in the MINX_ONE_CHUNK_LOAD_SIZE macro.
 *
 * This behaviour can be forbidden by defining the __MINIRAM__ macro.
 *
 * If it is bigger, it will be divided in 2 to n chunks. If __MINIRAM__ is defined,
 * this is 0.1% of the RAM-size, else it is 1% of the RAM-size.
 *
 * I think these values are really good. On a 2GiB-RAM machine, 0.1 is about
 * 2MiB, 1% is ~20 MiB. This is really much for a minx-binary AT THE MOMENT.
 * Later on, these values will change (especially if there is a compiler for a
 * high-level language, not this basic ASM-Stuff!)
 *
 * @return The max-size of a cache-chunk
 */
static long calculate_caching_size(long filesize) {
#ifndef __MINIRAM__
    if (filesize <= MINX_ONE_CHUNK_LOAD_SIZE) {
        return MINX_ONE_CHUNK_LOAD_SIZE;
    }

#else //__MINIRAM__



#endif //__MINIRAM__
}
