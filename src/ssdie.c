#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "header/ssdie.h"
#include "header/sql_functions.h"
#include "header/file_functions.h"



void extract_profile(sqlite3* db) {
    sqlite3_stmt* profile_stmt = get_sql_statement("select fullname, skypename, city, country, about FROM Accounts;",db);
    char * profile_file_pathname = concat_pathname_elements(pathname,"Profile");
    FILE* profilelog= fopen(profile_file_pathname,"w+");
    
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
    int stmt_result;    
    
    while((stmt_result = sqlite3_step(contact_stmt)) == SQLITE_ROW) {

        fprintf(contactlog,"\nFullName: %s and SkypeName: %s ",sqlite3_column_text(contact_stmt,0)
                                                              ,sqlite3_column_text(contact_stmt,1));
        check_ambig_text_column(contact_stmt,"PhoneNum",2,contactlog);  //Check if value exists at Skype Phone Number
        fprintf(contactlog,"%c",'\n');
    } 
   
    //Cleanup and Free memory
    free(contacts_file_pathname);
    free_sql_statement(contact_stmt);
    fclose(contactlog); 
}

void extract_calls(sqlite3* db) {
    sqlite3_stmt* call_stmt = get_sql_statement("select datetime(begin_timestamp, 'unixepoch'), identity from  Calls, Conversations where calls.conv_dbid = conversations.id",db);
    char* calls_file_pathname = concat_pathname_elements(pathname, "Calls");
    FILE* calllog = fopen(calls_file_pathname, "w+");
    int stmt_result;

    while((stmt_result = sqlite3_step(call_stmt)) == SQLITE_ROW) {
        fprintf(calllog,"%s","Call Extracted: \n");
        fprintf(calllog,"Beginning Time: %s\n",sqlite3_column_text(call_stmt,0));
        
       char* callname = sqlite3_column_text(call_stmt,1);
       if(*callname  == '#') { //If the first character is "#", this indicates a group chat
           fprintf(calllog, "Call With: %s\n", get_chat_name(callname));
           fprintf(calllog, "Participants of Call: %s", get_chat_participants(callname)); 
       } else {
           fprintf("Call with: %s", callname);
       }
        
    }
    free(calls_file_pathname);
    free_sql_statement(call_stmt);
    fclose(calllog);
}

void extract_messages(sqlite3* db) {
    char* messages_dir_name = "Messages";
    char* messages_dir_pathname = concat_pathname_elements(pathname, messages_dir_name);
    mkdir(messages_dir_pathname, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |  S_IROTH);

    sqlite3_stmt* messages_stmt = get_sql_statement("select datetime(timestamp, 'unixepoch'), from_dispname, dialog_partner, author, chatname, body_xml from Messages",db);
    int stmt_result;

    while((stmt_result = sqlite3_step(messages_stmt)) == SQLITE_ROW) {
        char* message = sqlite3_column_text(messages_stmt,5);
        char* chatname = get_chat_name(sqlite3_column_text(messages_stmt,4));
        char* author = sqlite3_column_text(messages_stmt, 3);
        char* partner = sqlite3_column_text(messages_stmt,2);
        char* displayname = sqlite3_column_text(messages_stmt,1);
        char* timestamp = sqlite3_column_text(messages_stmt,0);
        
        char* message_file_path = concat_pathname_elements(messages_dir_pathname, chatname);
        FILE* messagelog = fopen(message_file_path, "w+");
        
        if(!partner || (!strcmp(author,partner))) { //Checks if author and sender is the same person or if there is no partner
            fprintf(messagelog,"[%s] Send to User by  %s\n", timestamp, displayname); 
        } else {
            fprintf(messagelog,"[%s] Sent to: %s from %s\n", timestamp, partner,displayname);
        }
        fprintf(messagelog, "Message: %s",message);

        free(message_file_path); //Cleanup
        fclose(messagelog);
    }

   free(messages_dir_pathname);
   free_sql_statement(messages_stmt);
}

void check_ambig_text_column(sqlite3_stmt* stmt, char* columnname, int column, FILE* logfile) {
	if (sqlite3_column_type(stmt, column) != SQLITE_NULL) {
	    fprintf(logfile,"%s: %s\n", columnname, sqlite3_column_text(stmt,column));
	}
}

