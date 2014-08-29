#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

#include "header/ssdie.h"
#include "header/sql_functions.h"
#include "header/file_functions.h"


sqlite3* db;
char* pathname;

int main(int argc, char** argv) {

  
    if(argc != 3) {
      fprintf(stderr, "USAGE: ssdie [DB Path] [DIRPATH]");
      return EXIT_FAILURE;
    }
    
    char* dbname = argv[1];
    pathname = argv[2]; //Rootdir the directory in which the directory that will contain the result files will be contained in.
    
    db = open_sql_database(dbname);
    check_make_result_dir(pathname); //Create the directory

    printf("\nExtracting to directory: %s",pathname);
    printf("\n%s","Starting to extract Contacts"); //Begin extraction process. 
    extract_contacts(db);
    printf("\n%s\n","Starting to extract Personal Profile");
    extract_profile(db);
    printf("\n%s\n","Starting to extract Phone Calls");
    extract_calls(db);
    printf("\n%s\n","Starting to extract Messages");
    extract_messages(db);

    sqlite3_close(db); 
    return EXIT_SUCCESS;
}
