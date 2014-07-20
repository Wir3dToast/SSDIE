
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
      return 0;
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
            strcpy(fullname, sqlite3_column_text(stmt,0)); //Get First Column 
            strcpy(skypename,sqlite3_column_text(stmt,1)); //Get Second Column
                        printf("\nFullName: %s and SkypeName: %s",fullname,skypename);

             if(sqlite3_column_type(stmt,4) != SQLITE_NULL) { //Check if value exists at column for this row
                 
                 printf("\nType: %i", sqlite3_column_type(stmt,4)); //If so, 
                 strcpy(phonemobile, sqlite3_column_text(stmt,4)); 
                 printf("\nPhoneNum: %s",phonemobile); //This works
             }
        }
    } while (result == SQLITE_ROW);
    
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
        return NULL;
    }
    return stmt;
}

sqlite3* open_sql_database(char* dbname) {
    sqlite3* db;
    if((sqlite3_open(dbname,&db)) != SQLITE_OK) {
        fprintf(stderr, "Failed Open Database\n");
        sqlite3_close(db);
        return NULL;
    }
    return db;
}



