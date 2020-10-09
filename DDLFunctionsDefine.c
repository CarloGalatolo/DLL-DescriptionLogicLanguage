#define HASHSIZE 101
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>


typedef struct attr {
	char* name;
	char* type;
	int key;
} Attr;

char* loadedDB, loadedTAB;
int isLoaded = 0;

void setLoadedDB (char* str) {
	DIR* dir = opendir(str);
	if (dir) {	// Directory exists.
		loadedDB = str;
		isLoaded = 1;					
	}
	else	{
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
			check = mkdir(nomeDB, 0777);
		#endif
	  
		if (!check) {	// Check if directory is created or not.		
			printf("Database Creato e caricato!\n"); 
			loadedDB = nomeDB;
			isLoaded = 1;
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
			printf("Path cercato: %s \n",pathBuffer);
		} else {
			fclose(f);
			printf("Table Creata \n");
		}				
	} else {
		printf("ATTENZIONE: Database non caricato; caricare il DB con LOAD: prima di creare Tabelle\n");
	}		
}

int rimuoviDB (char* nomeDB) {
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


rimuoviTable (char* nomeTable){
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
		printf("Inserimento attributi. Database caricato: %s; Table scelta: %s.\n", loadedDB, table);	// DEBUG
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
			printf("Sono qui");
			char* token;
			Attr a;
			strcpy(a.name, "");
			strcpy(a.type, "");
			a.key = 0;

			while (token = strtok(chain, " ")) {
				if (strcmp(token, ",") == 0) {
					fprintf(f, "%s%s", a.name, a.type);
					if (a.key) fprintf(f, "*");
					fprintf(f, "||");
					strcpy(a.name, "");
					strcpy(a.type, "");
					a.key = 0;
				} else if (strcmp(token, "KEY") == 0) {
					a.key = 1;
				} else if (strcmp(token, "[STRINGA]") == 0 || strcmp(token, "[NUMERO]") == 0) {
					strcpy(a.type, token);
				} else {	// Nome dell'attributo
					strcpy(a.name, token);
				}
			}
			
			fclose(f);
		}
	} else {
		printf("ATTENZIONE: Database non caricato; caricare il DB con LOAD: prima di creare Tabelle\n");
	}
}


