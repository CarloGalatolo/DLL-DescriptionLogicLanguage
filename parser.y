%{	
	#include <stdio.h> 
	#include <string.h> 
	#include "DDLFunctionsDefine.h"	
%}

%union
{
    char *str;
}

%type <str> dest attr

%token <str> NOME 
%token <str> DIM 
%token <str> NUM 
%token <str> TYPE
%token KEY COMMA
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

LINGUAGGIO: C NOME DB EOL {	creaDB($2); }
		  | C NOME TAB EOL { creaTable($2); }
		  | C error EOL {yyerrok; yyclearin; printf("Per la sintassi completa usare help C: \n");}
		  | R NOME DB EOL { rimuoviDB($2); }
		  | R NOME TAB EOL  { }
		  | R error EOL		{yyerrok; yyclearin;printf("Per la sintassi completa usare help R: \n");}
		  | A dest attr EOL { appendAttr($2, $3); }
		  | A error EOL	  { yyerrok;
							printf("Il token [%s] e errato o non esistente\n",yylval);
							yyclearin;
							printf("Per la sintassi completa usare help A:\n"); }
;

/* Attributi della tabella */
attr: NOME TYPE					{ $$ = malloc(strlen($1) + strlen($2) + 2); strcpy($$, $1); strcat($$, " "); strcat($$, $2); }
	| NOME TYPE KEY				{ $$ = malloc(strlen($1) + strlen($2) + 6); strcpy($$, $1); strcat($$, " "); strcat($$, $2); strcat($$, " "); strcat($$, "KEY"); }
	| NOME TYPE COMMA			{ $$ = malloc(strlen($1) + strlen($2) + 4); strcpy($$, $1); strcat($$, " "); strcat($$, $2); strcat($$, " "); strcat($$, ","); }
	| NOME TYPE KEY COMMA		{ $$ = malloc(strlen($1) + strlen($2) + 8); strcpy($$, $1); strcat($$, " "); strcat($$, $2); strcat($$, " "); strcat($$, "KEY"); strcat($$, " "); strcat($$, ","); }
	| attr NOME TYPE			{ $$ = malloc(strlen($1) + strlen($2) + strlen($3) + 3); strcat($$, " "); strcat($$, $2); strcat($$, " "); strcat($$, $3); }
	| attr NOME TYPE KEY		{ $$ = malloc(strlen($1) + strlen($2) + strlen($3) + 7); strcat($$, " "); strcat($$, $2); strcat($$, " "); strcat($$, $3); strcat($$, " "); strcat($$, "KEY"); }
	| attr NOME TYPE COMMA		{ $$ = malloc(strlen($1) + strlen($2) + strlen($3) + 5); strcat($$, " "); strcat($$, $2); strcat($$, " "); strcat($$, $3); strcat($$, " "); strcat($$, ","); }
	| attr NOME TYPE KEY COMMA	{ $$ = malloc(strlen($1) + strlen($2) + strlen($3) + 9); strcat($$, " "); strcat($$, $2); strcat($$, " "); strcat($$, $3); strcat($$, " "); strcat($$, "KEY"); strcat($$, " "); strcat($$, ","); }
;

/* Destinazione di un comando */
dest: DB NOME TAB NOME { setLoadedDB($2); $$ = $4; }
	| TAB NOME		   { $$ = $2; }
;

%%

main (int argc, char **argv)	{
	yyparse();	
}

yyerror( char *s){
	fprintf(stderr, "Error:%s\n", s);
}	
