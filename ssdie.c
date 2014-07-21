
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "ssdie.h"

int main(int argc, char** argv) {

    char* dbname = argv[1];

    if(argc == 1) {
      fprintf(stderr, "No Skype main.db specified\n");
      return EXIT_FAILURE;
    }

    sqlite3* db = open_sql_database(dbname);
    printf("%s","Starting to extract Contacts");
    extract_contacts(db);
    
    sqlite3_close(db);
}

int extract_profile(sqlite3* db) {
    sqlite3_stmt* profile_stmt = get_sql_statement("select fullname, legnth(fullname), skypename, length(skypename), city, length(city), country, length(country), about FROM Accounts",db);
       
 
    return EXIT_SUCCESS; 
}

int extract_contacts(sqlite3* db) {
    sqlite3_stmt* contact_stmt = get_sql_statement("select displayname, length(displayname), skypename, length(skypename), phone_mobile, length(phone_mobile), datetime(lastonline_timestamp, 'unixepoch'), about from contacts",db);					  
    //SQL columns
    char* displayname;
    char* skypename;

    int result;    

    do {
        result = sqlite3_step(contact_stmt);

        if(result == SQLITE_ROW) {
	    displayname = alloc_sql_text_column(contact_stmt,1);
	    skypename = alloc_sql_text_column(contact_stmt,3);

            strcpy(displayname, sqlite3_column_text(contact_stmt,0)); //Get FullName
            strcpy(skypename,sqlite3_column_text(contact_stmt,2)); //Get SkypeName

            printf("\nFullName: %s and SkypeName: %s",displayname,skypename);

            check_ambig_text_column(contact_stmt,"PhoneNum",4,5);  //Check if value exists at Skype Phone Number 
             
            free(displayname);
	    free(skypename);
        }
    } while (result == SQLITE_ROW);
   
    //Cleanup and Free memory
    free_sql_statement(contact_stmt); 
    
    return EXIT_SUCCESS;  
}

void* alloc_sql_text_column(sqlite3_stmt* stmt, int column) {
	return malloc((sqlite3_column_int(stmt,column) + 1) * sizeof(char));
}

void check_ambig_text_column (sqlite3_stmt* stmt, char* columnname, int column, int column_with_length) {
	if (sqlite3_column_type(stmt, column) != SQLITE_NULL) {
		char* column_text = alloc_sql_text_column(stmt,column_with_length);
		strcpy(column_text, sqlite3_column_text(stmt,column));
		printf("%s: %s\n", columnname, column_text);
                free(column_text);
	}
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
    printf("%s","Opening SQL Database");
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

