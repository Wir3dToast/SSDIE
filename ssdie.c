#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "ssdie.h"

#if DEPR
#include ssdie_depre.h
#endif

FILE* logfile;

int main(int argc, char** argv) {

    char* dbname = argv[1];
  
    if(argc != 3) {
      fprintf(stderr, "USAGE: ssdie [DB Path] [FILENAME]");
      return EXIT_FAILURE;
    }

    logfile = fopen(argv[2],"ab+");
    
    sqlite3* db = open_sql_database(dbname);
    printf("\n%s","Starting to extract Contacts");
    extract_contacts(db);
    printf("\n%s\n","Starting to extract Personal Profile");
    extract_profile(db);

    sqlite3_close(db);
    close(logfile); 
    return EXIT_SUCCESS;
}

int extract_profile(sqlite3* db) {
    sqlite3_stmt* profile_stmt = get_sql_statement("select fullname, skypename, city, country, about FROM Accounts;",db);
    fprintf(logfile, "\n%s\n","PERSONAL PROFILE FOUND");
    
    if(sqlite3_step(profile_stmt) == SQLITE_ROW) {	
	fprintf(logfile,"\nFullName: %s\n",sqlite3_column_text(profile_stmt,0));
	fprintf(logfile,"\nSkypeName: %s\n",sqlite3_column_text(profile_stmt,1));
	fprintf(logfile,"\nCity: %s\n", sqlite3_column_text(profile_stmt,2));
	fprintf(logfile,"\nCountry: %s\n", sqlite3_column_text(profile_stmt,3));
 	fprintf(logfile,"\nAbout: %s\n", sqlite3_column_text(profile_stmt,4));
    }

    free_sql_statement(profile_stmt); 
    return EXIT_SUCCESS; 
}

int extract_contacts(sqlite3* db) {
    sqlite3_stmt* contact_stmt = get_sql_statement("select displayname, skypename, phone_mobile, datetime(lastonline_timestamp, 'unixepoch'), about from contacts",db);
    int result;    
    fprintf(logfile,"\n%s\n","PERSONAL CONTACTS FOUND");
    
    do {
        result = sqlite3_step(contact_stmt);

        if(result == SQLITE_ROW) {
            fprintf(logfile,"\nFullName: %s and SkypeName: %s ",sqlite3_column_text(contact_stmt,0)
                                                              ,sqlite3_column_text(contact_stmt,1));
            check_ambig_text_column(contact_stmt,"PhoneNum",2);  //Check if value exists at Skype Phone Number
            fprintf(logfile,"%c",'\n');
        }
    } while (result == SQLITE_ROW);
   
    //Cleanup and Free memory
    free_sql_statement(contact_stmt); 
    return EXIT_SUCCESS;  
}

void check_ambig_text_column (sqlite3_stmt* stmt, char* columnname, int column) {
	if (sqlite3_column_type(stmt, column) != SQLITE_NULL) {
	    fprintf(logfile,"%s: %s\n", columnname, sqlite3_column_text(stmt,column));
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
#if DEPRE

void* alloc_sql_text_column(sqlite3_stmt* stmt, int column) {
	return malloc((sqlite3_column_int(stmt,column) + 1) * sizeof(char));
}

#endif 
