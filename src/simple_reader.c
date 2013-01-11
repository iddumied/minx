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

void minx_binary_init(FILE *f) {
    filesize = get_file_size(f);
    binary = (char*) malloc( sizeof(char) * filesize );
    size_t readsize = 1024;
    size_t lastreadsize;
    unsigned int readcount = 0;
    do {
        lastreadsize = fread( &(binary[readcount * readsize]), sizeof(char), readsize, f);
        readcount++;
    } while( readsize == lastreadsize );

#ifdef DEBUG
    printf("This is only print in dev version with simple_reader"
            "instead of the real binary_reader!\tBinary:\n");
    unsigned int i;
    for( i = 0 ; i < filesize; i++ ) {
        if( (int)binary[i] < 10 && (int)binary[i] >= 0) {
            printf(" %i ", (int)binary[i]);
        }
        else {
            printf("%i ", (int)binary[i]);
        }

        if( i % 8 == 0 ) {
            printf("\n");
        }
    }
    printf("\n");
#endif 

} 

void minx_binary_shutdown() {
    free(binary);
}

void * minx_binary_get_at(uint64_t p, unsigned int number_of_bytes) {
    char *res = malloc( sizeof(char) * number_of_bytes );
    memcpy(res, &binary[p], number_of_bytes);
    return res;
}

signed int minx_binary_exists_at(uint64_t p) {
    return filesize > p;
}

/*
 * Static functions implementations
 */

static long get_file_size(FILE *f) {
    long res;
    fseek(f, 0L, SEEK_END);
    res = ftell(f);
    fseek(f, 0L, SEEK_SET);
    return res;
}
