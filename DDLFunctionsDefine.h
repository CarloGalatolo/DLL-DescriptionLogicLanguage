#define HASHSIZE 101

char* loadedDB;
int isLoaded;

void setLoadedDB (char*);

void creaDB(char* nomeDB);
void creaTable(char* nomeTable);

int rimuoviDB(char* nomeDB);

void appendAttr (char*, char*);
