#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

#include "header/file_functions.h"

void check_make_result_dir(char* pathname) {
    int status = mkdir(pathname, S_IRUSR | S_IWUSR | S_IXUSR);
    
    switch(status) {
	case EEXIST: printf("%s","Directory of this name already exists");
		     exit(EXIT_FAILURE);
        case ENOENT: printf("%s","Pathname does not exist or Path is empty");
		     exit(EXIT_FAILURE);
        case ENOTDIR: printf("%s", "Component of the parth prefix is not a directory");
		      exit(EXIT_FAILURE);
    }
}

char* concat_pathname_elements(char* pathname, char* filename) {
    size_t len_of_pathname = strlen(pathname);
    size_t len_of_file = strlen(filename);

    char* full_pathname = malloc(len_of_pathname + len_of_file + 2 * sizeof(char)); //Plus 2 for end-of-string terminator and "/"
      
    memcpy(full_pathname,pathname,len_of_pathname);
    memset(full_pathname + len_of_pathname,'/',1);
    memcpy(full_pathname + len_of_pathname + 1, filename, len_of_file + 1);

    return full_pathname;

}

