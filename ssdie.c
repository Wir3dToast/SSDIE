
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "ssdie.h"

char * dbname; 

int main(int argc, char** argv) {

    dbname = argv[1];

    if(argc == 1) {
      fprintf(stderr, "No Skype main.db specified\n");
      return EXIT_FAILURE;
    }

    sqlite3* db = open_sql_database(dbname);
    extract_contacts(db);
    
    sqlite3_close(db);
}

int extract_contacts(sqlite3* db) {
    sqlite3_stmt* stmt = get_sql_statement("select displayname, skypename, phone_mobile, datetime(lastonline_timestamp, 'unixepoch'), about from contacts",
					  db);
					  
    char* fullname = malloc(100 * sizeof(char));
    char* skypename = malloc(100 * sizeof(char));
    char* phonemobile = malloc(100 * sizeof(char));
    
    int result;    
    do {
        result = sqlite3_step(stmt);
        if(result == SQLITE_ROW) {
            strcpy(fullname, sqlite3_column_text(stmt,0)); //Get FullName
            strcpy(skypename,sqlite3_column_text(stmt,1)); //Get SkypeName
                        printf("\nFullName: %s and SkypeName: %s",fullname,skypename);

             if(sqlite3_column_type(stmt,4) != SQLITE_NULL) { //Check if value exists at Skype Phone Number 
                  
                 strcpy(phonemobile, sqlite3_column_text(stmt,4)); 
                 printf("\nPhoneNum: %s",phonemobile); //Get Skype Phone Number
             }
        }
    } while (result == SQLITE_ROW);
   
    //Cleanup and Free memory
    free_sql_statement(stmt); 
    free(fullname);
    free(skypename);
    free(phonemobile);
    
    return EXIT_SUCCESS;  
}

sqlite3_stmt* get_sql_statement(char* sqlstmt, sqlite3* db) { 
    sqlite3_stmt* stmt;
    if ((sqlite3_prepare_v2(db, sqlstmt, strlen(sqlstmt) + 1, &stmt, NULL)) 
							    != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare Database\n");
        sqlite3_close(db);
        exit(0);
    }
    return stmt;
}

sqlite3* open_sql_database(char* dbname) {
    sqlite3* db;
    if((sqlite3_open(dbname,&db)) != SQLITE_OK) {
        fprintf(stderr, "Failed Open Database\n");
        sqlite3_close(db);
        exit(0);
    }
    return db;
}

void free_sql_statement(sqlite3_stmt* stmt_to_free) {
    if((sqlite3_finalize(stmt_to_free)) != SQLITE_OK) {
	fprintf(stderr,"ERROR: Freeing SQL Statement Memory Failed");
	exit(0);
    }
}

