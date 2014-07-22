//Extractor prototypes
int extract_contacts(sqlite3*);
int extract_profile(sqlite3*);

void check_ambig_text_column_(sqlite3_stmt*, char*,int, FILE*);