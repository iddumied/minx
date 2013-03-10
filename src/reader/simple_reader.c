#include "binary_reader.h"

/*
 * Static variables
 */ 

static char * binary;
static long filesize;

/*
 * Static function prototypes
 */

static long get_file_size(FILE *f);

/*
 * Functions
 */

/**
 * @brief Init the simple reader
 *
 * @param f Init with this file
 */
void minx_binary_init(FILE *f) {

    minx_error_register_shutdown_function(minx_binary_shutdown);

    filesize = get_file_size(f);
    binary = (char*) malloc( sizeof(char) * filesize );
    size_t readsize = 1024;
    size_t lastreadsize;
    unsigned int readcount = 0;
    do {
        lastreadsize = fread( &(binary[readcount * readsize]), sizeof(char), readsize, f);
        readcount++;
    } while( readsize == lastreadsize );
} 

/**
 * @brief Shutdown the Binary
 */
void minx_binary_shutdown() {
    free(binary);
}

/**
 * @brief Get values from the binary at a specific position
 *
 * @param p Pointer to the location to get data from
 * @param number_of_bytes Number of bytes to get
 * @param dest Destination of the data
 * @param destsize Size of the destination
 *
 * @return A pointer to the destination of the Data
 */
void * minx_binary_get_at(  uint64_t        p, 
                            unsigned int    number_of_bytes, 
                            void            *dest, 
                            size_t          destsize) {

    memset(dest, 0x00, destsize);
    memcpy(dest, &binary[p], number_of_bytes);
    return dest;
}

signed int minx_binary_exists_at(uint64_t p) {
    return filesize > p;
}

#ifdef DEBUGGING
/**
 * @brief Print the binary
 */
void minx_binary_print() {
    long i;
    unsigned int line = 0;
    printf("0x00000000 : ");
    for(i = 0 ; i < filesize ; i++ ) {
        if( binary[i] == 0 ) {
            printf("0x00000000 ");
        }
        else {
            printf("%#010x ", binary[i]);
        }
        if((i+1) % 8 == 0) {
            printf("\n%#010x : ", ++line);
        }
    } 
    printf("\n");
}
#endif //DEBGUGGING

/*
 * Static functions implementations
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
