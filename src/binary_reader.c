#include "binary_reader.h"

/*
 * Binary reader should only read until jump to 0xFFFF is read. But this is not
 * possible, because the binary_reader does not know, what it's dealing with. It
 * could be a variable value, huh?
 *
 * because of this, there is the 'read_until_end_of_file' variable. The pointer
 * 'p' should never point to a location outside of the binary file. The binary
 * must ensure that. 
 * The VM has to ensure, that if there is a jump to 0xFFFF, the shutdown is
 * initialized. 
 */

/*
 * static variables
 */

static int              binary_init     = 0;
static uint64_t         binary_size     = 0;
static char     *       binary          = NULL;
static uint64_t         alloc_step_size = 1024;
static int              read_until_end_of_file;

static FILE     *       file;

/*
 * static function prototypes 
 */

static void     init_binary     (void);
static void     read_until      (uint64_t p);

/*
 * Functions
 */ 

void minx_binary_init(FILE *f) {
    binary_init     = 0;
    binary_size     = 0;
    binary          = NULL;
    alloc_step_size = 1024;
    read_until_end_of_file     = 0;
    file            = f;
}

void minx_binary_shutdown() {
    free(binary);
    fclose(file);
}

/*
 * returns 'number_of_bytes' bytes 
 *  (currently, the vm needs 2 for commands and 8 for parameters)
 */
void * minx_binary_get_at(uint64_t p, unsigned int number_of_bytes) {

    void * result = (void*) malloc( sizeof(char) * number_of_bytes );
    
    if ( number_of_bytes > 8 ) {
        FATAL_DESC_ERROR("The VM supports only 64 Bit");
    }

    if( !binary_init ) {
        init_binary();
    }

    if( p > binary_size && read_until_end_of_file ) {
        FATAL_DESC_ERROR("ptr points outside of binary!");
    }
    else if ( p > binary_size && !read_until_end_of_file ) {
        read_until(p);
    }

    result = memcpy(result, &(binary[p]), sizeof(char)*number_of_bytes); 
    return result;
}


/*
 * returns true if binary exists at address 'p'
 */
signed int minx_binary_exists_at(uint64_t p) {
    if( binary_size >= p ) {
        return 1;
    }
    else {
        if( read_until_end_of_file )
            return 0;
        else {
            read_until(p);
            return minx_binary_exists_at(p);
        }
    }
}

/*
 * static functions 
 * ---------------
 */

/*
 * Init variable 'binary' with the first 'alloc_step_size' bytes from the file.
 */
static void init_binary() {
    binary_size += alloc_step_size;
    binary = (char *) malloc(sizeof(char) * binary_size);
    
    size_t read_size;
    read_size = fread( &(binary[binary_size]), sizeof(char), alloc_step_size, file );

    read_until_end_of_file = read_size == 0 || read_size != alloc_step_size;
}

/*
 * @param p the pointer to read until
 *
 * Read binary until 'p' points to a location inside of the binary or read until
 * end of the binary and set the 'read_until_end_of_file' variable to true.
 */
static void read_until(uint64_t p) {
    size_t read_size;
    while( p > binary_size || !read_until_end_of_file ) {
        binary      = realloc(binary, (binary_size+alloc_step_size) * sizeof(char));
        read_size   = fread( &(binary[binary_size]), sizeof(char), alloc_step_size, file );

        /*
         * fread returns zero or short item count if EOF or Error 
         */
        read_until_end_of_file = read_size == 0 || read_size != alloc_step_size;

        binary_size += alloc_step_size;
    }
}
