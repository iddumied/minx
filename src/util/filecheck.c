#include "util/filecheck.h"

/**
 * @brief 
 *
 * @param path
 *
 * @return 
 */
signed int minx_util_filecheck_file_exists(char *path) {
    return access(path, F_OK) != -1;
}

/**
 * @brief 
 *
 * @param path
 *
 * @return 
 */
signed int minx_util_filecheck_path_is_file(char *path) {

    /*
     * check if given path is a file
     */
    struct stat s;
    if(stat(path, &s) == 0) {
        if(S_ISREG(s.st_mode)) {
            /* everything ok */
            return 1;
        }
        else if(S_ISDIR(s.st_mode)) {
            printf("Passed argument is a directory!\n");
            return 0;
        }
        else {
            printf("Passed argument is no file!\n");
            return 0;
        }
    }
    /* else  */
    /* error */
    printf("Error reading stats from passed path!\n");
    return 0;
}

/**
 * @brief 
 *
 * @param path
 *
 * @return 
 */
signed int minx_util_filecheck_file_is_readable(char *path) {
    return access(path, R_OK) != -1;
}

/**
 * @brief Handle the error when the binary file could not be opened
 */
void handle_file_open_error() {
    perror("Error opening file");
}
