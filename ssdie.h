//SQLite3 prototypes
sqlite3_stmt* get_sql_statement(char *,sqlite3*); 
sqlite3* open_sql_database(char *);
void free_sql_statement(sqlite3_stmt*);
void check_ambig_text_column_(sqlite3_stmt*, char*,int);

//Extractor prototypes
int extract_contacts(sqlite3*);
int extract_profile(sqlite3*);
