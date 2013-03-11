#ifndef __MINX_UTIL_FILECHECK_H__
#define __MINX_UTIL_FILECHECK_H__

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

signed int  minx_util_filecheck_file_exists         (char *path);
signed int  minx_util_filecheck_path_is_file        (char *path);
signed int  minx_util_filecheck_file_is_readable    (char *path);

void        handle_file_open_error                  (void);

#endif//__MINX_UTIL_FILECHECK_H__
