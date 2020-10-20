#define HASHSIZE 101
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>


char* loadedDB, loadedTAB;
int isLoaded = 0;

void myToUpper (char* str) {
	int i;

	for (i = 0; i < strlen(str); i++) {
		str[i] = toupper(str[i]);
	}
}

void setLoadedDB (char* str) {
	if (opendir(str)) {	// Directory exists.
		loadedDB = str;
		isLoaded = 1;					
	} else {
		printf("ERRORE: Database non esistente!\n");  
	}
}

void creaDB (char* nomeDB) {
	int check; 
	DIR* dir = opendir(nomeDB);
	if (dir) {	// Directory exists.
		closedir(dir);
		printf("ATTENZIONE: Database Esistente con nome [%s], usare LOAD: per caricare il Database\n", nomeDB);				
	} else if (ENOENT == errno) {
		#if defined(_WIN32)
			check = _mkdir(nomeDB);
		#else 
			check = mkdir(nomeDB, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		#endif
	  
		if (!check) {	// Check if directory is created or not.		
			printf("Database Creato e caricato!\n"); 
			setLoadedDB(nomeDB);
		} else { 
			printf("ERRORE: Non e stato possibile creare il Database\n");   	 	
		} 
	}
}

void creaTable (char* nomeTable) {
	if (isLoaded == 1) {	// DB exists.
		printf("Database Esistente con nome [%s], Creazione Table [%s] \n", loadedDB, nomeTable);
		FILE *f;
		char pathBuffer[PATH_MAX + 1];

		strcpy(pathBuffer, "./");
		strcat(pathBuffer, loadedDB);
		strcat(pathBuffer, "/");
		strcat(pathBuffer, nomeTable);
		strcat(pathBuffer, ".txt");

		//f = fopen(loadedDB, "a");

		if ( (f = fopen(pathBuffer, "a")) == NULL ) {
			printf("Table NON Creata \n");
			printf("Path cercato: %s \n", pathBuffer);
		} else {
			fclose(f);
			printf("Table Creata \n");
		}				
	} else {
		printf("ATTENZIONE: Database non caricato; caricare il DB con LOAD: prima di creare Tabelle\n");
	}		
}

int  rimuoviDB (char* nomeDB) {
	int check; 
	DIR* dir = opendir(nomeDB);

	if (dir) {	// Directory exists.
		if (nomeDB == loadedDB)	isLoaded = 0;								 
		size_t path_len = strlen(nomeDB);
		int r = -1;
	
		if (dir) {
			struct dirent *p;				 	
			r = 0;
			while (!r && (p=readdir(dir))) {
				int r2 = -1;
				char *buf;
				size_t len;					
				/* Skip the names "." and ".." as we don't want to recurse on them. */
				if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")){			
					continue;
				}					        
				len = path_len + strlen(p->d_name) + 2; 
				buf = malloc(len);
	
				if (buf) {
					struct stat statbuf;
	
					snprintf(buf, len, "%s/%s", nomeDB, p->d_name);
					if (!stat(buf, &statbuf)) {
						if (S_ISDIR(statbuf.st_mode)) r2 = rimuoviDB(buf);
						else r2 = unlink(buf);
					}
					free(buf);
				}
				r = r2;
			}
			closedir(dir);
		}
	
		if (!r)	r = rmdir(nomeDB);							 						 	
		return r;						  									 						
	} else if (ENOENT == errno) {	// Check if directory is created or not.
		printf("ERRORE: Database non esistente\n");   	 	
	} 
}

void rimuoviTable (char* nomeTable){
	if (isLoaded == 1) {	// DB exists.
		printf("Database Esistente con nome [%s], Rimozione Table [%s] \n", loadedDB, nomeTable);
		FILE *f;
		char pathBuffer[PATH_MAX + 1];

		strcpy(pathBuffer, "./");
		strcat(pathBuffer, loadedDB);
		strcat(pathBuffer, "/");
		strcat(pathBuffer, nomeTable);
		strcat(pathBuffer, ".txt");

		//f = fopen(loadedDB, "a");

		if ( (f = fopen(pathBuffer, "r")) == NULL ) {
			printf("Table Non esistente \n");
		
		} else {
			fclose(f);
			remove(pathBuffer);
			printf("Table Rimossa \n");
		}				
	} else {
		printf("ATTENZIONE: Database non caricato; caricare il DB con LOAD: prima di creare Tabelle\n");
	}

}

void appendAttr (char* table, char* chain) {
	if (isLoaded == 1) {	// DB exists.
		FILE *f;

		// Creazione stringa di PATH
		char pathBuffer[PATH_MAX + 1];
		strcpy(pathBuffer, "./");
		strcat(pathBuffer, loadedDB);
		strcat(pathBuffer, "/");
		strcat(pathBuffer, table);
		strcat(pathBuffer, ".txt");

		if ( (f = fopen(pathBuffer, "a")) == NULL ) {
			printf("ERRORE: tabella non trovata.");
		} else {
			char* token = (char*) malloc(100);
			char* name = (char*) malloc(100);
			char* type = (char*) malloc(100);
			strcpy(name, "");
			strcpy(type, "");
			int key = 0;

			token = strtok(chain, "|");
			while (token) {
				if (strcmp(token, ",") == 0) {
					fprintf(f, "%s%s", name, type);
					if (key) fprintf(f, "*");
					fprintf(f, "|");
					strcpy(name, "");
					strcpy(type, "");
					key = 0;
				} else if (strcmp(token, "KEY") == 0) {
					key = 1;
				} else if (strcmp(token, "[STRINGA]") == 0 || strcmp(token, "[NUMERO]") == 0) {
					strcpy(type, token);
				} else {	// Nome dell'attributo
					strcpy(name, token);
				}
				
				token = strtok(0, "|");
			}

			fprintf(f, "%s%s", name, type);
			if (key) fprintf(f, "*");
			fprintf(f, "|");
			
			fclose(f);
			free(token);
			free(name);
			free(type);
		}
	} else {
		printf("ATTENZIONE: Database non caricato; caricare il DB con LOAD: prima di creare Tabelle\n");
	}
}


void countColsRows(FILE* db, int* row, int* col){
	
	char ch;
	int numrows=0,numcols=0;
	
	while((ch=fgetc(db))!=EOF) {
      if(ch=='|' && numrows==0)
         numcols++;
	  if(ch=='\n')
         numrows++;
   }
	
};


tableLoader(char*** DBTable, int* DBStruct, FILE* tableFile){
	int bufferLength = 255;
	char buffer[bufferLength];
	char* strrFinderTypeStart;
	char* strrfinderTypeEnd;
	char* strrfinderIsKey;
	
	char characterStart = '[';
	char characterEnd = ']';
	char characterKey = '*';
	int colCounter=0;
	int rowCounter=0;
	
while(fgets(buffer, bufferLength, tableFile)) {
     	char* token = (char*) malloc(100);		 
		token = strtok(buffer, "|");
     	while (token) {    		
			strrFinderTypeStart=strrchr( token, characterStart);
			strrFinderTypeEnd=strrchr( token, characterEnd );	
			strrFinderIsKey=strrchr( token, characterKey);	
			
			if (strrFinderTypeStart)
			strncpy(DBTable[rowCounter][colCounter][],token, 5)
			if (strrFinderIsKey)			
			token = strtok(0, "|");
			}
}

};


void appendTuple(char* table, char* chain){
	if (isLoaded == 1) {	// DB exists.
		FILE *f;

		// Creazione stringa di PATH
		char pathBuffer[PATH_MAX + 1];
		strcpy(pathBuffer, "./");
		strcat(pathBuffer, loadedDB);
		strcat(pathBuffer, "/");
		strcat(pathBuffer, table);
		strcat(pathBuffer, ".txt");

		if ( (f = fopen(pathBuffer, "a+")) == NULL ) {
			printf("ERRORE: tabella non trovata.");
		} else {
				
  					int rows,cols;
  					countColsRows(&f,&rows,&cols);
  					rewind(f);
  					char ***DBMatrix;
  					int  DBStructure[cols];
  					DBMatrix=malloc(rows * cols * 100 * sizeof(char)); 
  					tableLoader(&DBStructure,&DBMatrix, &f); 
  								 					
  				
				  		
		
		}
	
	
	
	
}

