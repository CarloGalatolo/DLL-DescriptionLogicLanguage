#define HASHSIZE 101
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>


char* loadedDB;
int isLoaded=0;

void creaDB(char* nomeDB){
		int check; 
		DIR* dir = opendir(nomeDB);
		if (dir) {
		 			 /* Directory exists. */
		 			closedir(dir);
   				 	printf("ATTENZIONE: Database Esistente con nome [%s], usare LOAD: per caricare il Database\n", nomeDB);				
					}
		
		else if (ENOENT == errno){
			  	check = mkdir(nomeDB); 
  			  /*check if directory is created or not */
  	  	if (!check) {			
    	    printf("Database Creato e caricato!\n"); 
    	    loadedDB=nomeDB;
    	    isLoaded=1;
    	}else { 
      		printf("ERRORE: Non è stato possibile creare il Database\n");   	 	
    } 
}
}


void creaTable(char* nomeTable){
		
		if (isLoaded==1) {
		 			 /* DB exists.*/
		   				 	printf("Database Esistente con nome [%s],Creazione Table [%s] \n",loadedDB,nomeTable);		   				
		   				 	FILE *f;
		   				 	char pathBuffer[PATH_MAX+1];
		   				 	strcpy(pathBuffer, "./");
		   				 	strcat(pathBuffer, loadedDB);
		   				 	strcat(pathBuffer,"/" );
		   				 	strcat(pathBuffer, nomeTable);
		   				 	strcat(pathBuffer, ".txt");
							f = fopen(loadedDB, "a");
							if ((f=fopen(pathBuffer, "a"))==NULL){
								printf("Table NON Creata \n");
								printf("Path cercato: %s \n",pathBuffer);
							}
							else{
								fclose(f);
								printf("Table Creata \n");
							}				
					}else{
						  printf("ATTENZIONE: Database non caricato; caricare il DB con LOAD: prima di creare Tabelle\n");
					}		
}


void rimuoviDB(char* nomeDB){
		int check; 
		DIR* dir = opendir(nomeDB);
		if (dir) {
		 			 /* Directory exists. */
		 			closedir(dir);
					if( nomeDB==loadedDB)
						isLoaded=0;							 			
   				 	printf("ATTENZIONE: Database Esistente con nome [%s], usare LOAD: per caricare il Database\n", nomeDB);				
					}
		
		else if (ENOENT == errno){
			  	check = mkdir(nomeDB); 
  			  /*check if directory is created or not */
  	  	if (!check) {			
    	    printf("Database Creato e caricato!\n"); 
    	    loadedDB=nomeDB;
    	    isLoaded=1;
    	}else { 
      		printf("ERRORE: Non è stato possibile creare il Database\n");   	 	
    } 
}
}








