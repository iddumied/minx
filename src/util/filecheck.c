#include "util/filecheck.h"

/**
 * @brief Check if a file exists
 *
 * @param path the path to the file
 *
 * @return true if the file exists, else false
 */
signed int minx_util_filecheck_file_exists(char *path) {
    return access(path, F_OK) != -1;
}

/**
 * @brief Check if a path is a file or something else
 *
 * @param path The path to check
 *
 * @return true if the path is a file, else false
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
 * @brief Check if the file under the path is readable
 *
 * @param path The path to the file
 *
 * @return true if the file is readable, else false
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
