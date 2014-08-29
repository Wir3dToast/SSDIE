#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "header/auxiliary.h"
#include "header/sql_functions.h"


char* get_chat_name(char* chatname) {
    char* chat_sql_statement = strdup("select friendlyname from Chats where name =\"");
    asprintf(&chat_sql_statement, "%s%s\" limit 1",chat_sql_statement,chatname);
    
    sqlite3_stmt* chat_stmt = get_sql_statement(chat_sql_statement,db);
    int stmt_result;    
    char* column_result = ((stmt_result = sqlite3_step(chat_stmt)) == SQLITE_ROW) ? sqlite3_column_text(chat_stmt,0)    	
                                                                                  : "Nonexistent Chat";
    free_sql_statement(chat_stmt);
    free(chat_sql_statement);
    
    return column_result;                                                                                                                             
}

char* get_chat_participants(char* chatname) {
    char* participants_sql_stmt = strdup("select participants from Chats where name=\"");
    asprintf(&participants_sql_stmt, "%s%s\" limit 1", participants_sql_stmt, chatname);

    sqlite3_stmt* participants_stmt = get_sql_statement(participants_sql_stmt,db);
    int stmt_result;
    char* column_result = NULL;
    if((stmt_result = sqlite3_step(participants_stmt)) == SQLITE_ROW) {
        column_result = sqlite3_column_text(participants_stmt,0);
    } 
    free_sql_statement(participants_stmt);
    free(participants_sql_stmt);
    return column_result;

}
