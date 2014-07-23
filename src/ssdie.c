#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include "header/ssdie.h"
#include "header/sql_functions.h"
#include "header/file_functions.h"


sqlite3* db;
char* pathname;

int main(int argc, char** argv) {

  
    if(argc != 4) {
      fprintf(stderr, "USAGE: ssdie [DB Path] [DIRNAME] [DIRPATH]");
      return EXIT_FAILURE;
    }
    
    char* dbname = argv[1];
    char* rootdir = argv[3]; //Rootdir the directory in which the directory that will contain the result files will be contained in.
    
    db = open_sql_database(dbname);
  
    pathname = concat_pathname_elements(rootdir,argv[2]); //Create the pathname for the directory that will contain the result files. 
    check_make_result_dir(pathname); //Create the directory

    printf("\nExtracting to directory: %s",pathname);
    printf("\n%s","Starting to extract Contacts"); //Begin extraction process. 
    extract_contacts(db);
    printf("\n%s\n","Starting to extract Personal Profile");
    extract_profile(db);

    free(pathname); 
    sqlite3_close(db); 
    return EXIT_SUCCESS;
}

void extract_profile(sqlite3* db) {
    sqlite3_stmt* profile_stmt = get_sql_statement("select fullname, skypename, city, country, about FROM Accounts;",db);
    char * profile_file_pathname = concat_pathname_elements(pathname,"Profile");
    FILE* profilelog= fopen(profile_file_pathname,"w+");
    
    fprintf(profilelog, "\n%s\n","PERSONAL PROFILE FOUND");
    
    if(sqlite3_step(profile_stmt) == SQLITE_ROW) {	
	fprintf(profilelog,"\nFullName: %s\n",sqlite3_column_text(profile_stmt,0));
	fprintf(profilelog,"\nSkypeName: %s\n",sqlite3_column_text(profile_stmt,1));
	fprintf(profilelog,"\nCity: %s\n", sqlite3_column_text(profile_stmt,2));
	fprintf(profilelog,"\nCountry: %s\n", sqlite3_column_text(profile_stmt,3));
 	fprintf(profilelog,"\nAbout: %s\n", sqlite3_column_text(profile_stmt,4));
    }
    
    //Cleanup and Free memory
    free(profile_file_pathname);
    free_sql_statement(profile_stmt); 
    fclose(profilelog); 
}

void extract_contacts(sqlite3* db) {
    sqlite3_stmt* contact_stmt = get_sql_statement("select displayname, skypename, phone_mobile, datetime(lastonline_timestamp, 'unixepoch'), about from contacts",db);
    char* contacts_file_pathname = concat_pathname_elements(pathname,"Contacts");
    FILE* contactlog = fopen(contacts_file_pathname,"w+");
    int result;    

    fprintf(contactlog,"\n%s\n","PERSONAL CONTACTS FOUND");
    
    do {
        result = sqlite3_step(contact_stmt);

        if(result == SQLITE_ROW) {
            fprintf(contactlog,"\nFullName: %s and SkypeName: %s ",sqlite3_column_text(contact_stmt,0)
                                                              ,sqlite3_column_text(contact_stmt,1));
            check_ambig_text_column(contact_stmt,"PhoneNum",2,contactlog);  //Check if value exists at Skype Phone Number
            fprintf(contactlog,"%c",'\n');
        }
    } while (result == SQLITE_ROW);
   
    //Cleanup and Free memory
    free(contacts_file_pathname);
    free_sql_statement(contact_stmt);
    fclose(contactlog); 
}

void check_ambig_text_column(sqlite3_stmt* stmt, char* columnname, int column, FILE* logfile) {
	if (sqlite3_column_type(stmt, column) != SQLITE_NULL) {
	    fprintf(logfile,"%s: %s\n", columnname, sqlite3_column_text(stmt,column));
	}
}
