#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "header/sql_functions.h"

#if DEPR
#include ssdie_depre.h
#endif

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