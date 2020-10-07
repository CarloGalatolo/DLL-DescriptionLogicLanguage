#define HASHSIZE 101

char* loadedDB;
int isLoaded;

void creaDB(char* nomeDB);
void creaTable(char* nomeTable);
int rimuoviDB(char* nomeDB);

void appendAttrDB (char*, char*, char*, char*);
void appendAttrTAB (char*, char*, char*);
