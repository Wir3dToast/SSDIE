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
    char *filepathname = strdup(pathname);
    asprintf(&filepathname, "%s/%s", filepathname,filename);
    return filepathname;

}

