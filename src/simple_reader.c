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

void minx_binary_shutdown() {
    free(binary);
}

void * minx_binary_get_at(uint64_t p, unsigned int number_of_bytes, void *dest) {
    memcpy(dest, &binary[p], number_of_bytes);
    return dest;
}

signed int minx_binary_exists_at(uint64_t p) {
    return filesize > p;
}

#ifdef DEBUGGING
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

static long get_file_size(FILE *f) {
    long res;
    fseek(f, 0L, SEEK_END);
    res = ftell(f);
    fseek(f, 0L, SEEK_SET);
    return res;
}
