sqlite3_stmt* get_sql_statement(char *,sqlite3*); 
sqlite3* open_sql_database(char *);

int extract_contacts(sqlite3*);
