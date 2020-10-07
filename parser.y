%{	
	#include <stdio.h> 
	#include <string.h> 
	#include "DDLFunctionsDefine.h"	
%}

%union 
{
        char *str;
}


%token <str> NOME 
%token <str> DIM 
%token <str> NUM 
%token <str> TYPE
%token C R A D S DA W L DB TAB EXIT 
%token EOL
%token VAR
%error-verbose

%%

PROGRAM: PROGRAM LINGUAGGIO
|LINGUAGGIO 
|NUM {printf("Yuppie ka ye");}
|VAR {printf("Yuppws");}
|error EOL{yyerrok; yyclearin;printf("Comando non riconosciuto, usare help per vedere la lista di comandi \n");}
;

LINGUAGGIO:DDL
|DDL2
;

DDL:C NOME DB EOL {	creaDB($2); }
|C NOME TAB EOL { creaTable($2); }
|C error EOL {yyerrok; yyclearin; printf("Per la sintassi completa usare help C: \n");}
|R NOME DB EOL { }
|R NOME TAB EOL { }
|R error EOL{yyerrok; yyclearin;printf("Per la sintassi completa usare help R: \n");}
;

DDL2: A dest attr EOL { printf("Aggiunto attributo a tabella.\n"); }
	| A error EOL	  { yyerrok;
						printf("Il token [%s] e errato o non esistente\n",yylval);
						yyclearin;
						printf("Per la sintassi completa usare help A:\n"); }
;

/* Attributo della tabella */
attr: NOME TYPE		 { printf("Trovato attributo: %s %s\n", $1, $2); }
	| attr NOME TYPE { printf("Trovata catena di attributi. Ultimo: %s %s\n", $2, $3); }
;

/* Destinazione di un comando */
dest: DB NOME TAB NOME {}
	| TAB NOME {}
;

%%

main (int argc, char **argv)	{
	yyparse();	
}

yyerror( char *s){
	fprintf(stderr, "Error:%s\n", s);
}	
