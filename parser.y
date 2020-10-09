%{	
	#include <stdio.h> 
	#include <string.h> 
	#include "DDLFunctionsDefine.h"	
%}

%union
{
    char *str;
}

%type <str> db tab dest attr

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

LINGUAGGIO: LINGUAGGIO EOL {}
		  | create {}
		  | remove {}
		  | A dest attr  { appendAttr($2, $3); }
		  | A error 	    { yyerrok;
							  printf("Il token [%s] e errato o non esistente\n",yylval);
							  yyclearin;
							  printf("Per la sintassi completa usare help A:\n"); }
;

/* Attributo della tabella */
attr: NOME TYPE		  			{ myToUpper($2); sprintf($$, "%s|%s", $1, $2); }
	| NOME TYPE KEY	  			{ myToUpper($2); sprintf($$, "%s|%s|KEY", $1, $2); }
	| attr COMMA NOME TYPE		{ myToUpper($4); sprintf($$, "%s|,|%s|%s", $1, $3, $4); }
	| attr COMMA NOME TYPE KEY	{ myToUpper($4); sprintf($$, "%s|,|%s|%s|KEY", $1, $3, $4); }
;

/* Destinazione di un comando */
dest: db tab { setLoadedDB($1); $$ = $2;}
	| tab		   { $$ = $1; }
;

db: DB NOME		{ $$ = $2; }
;

tab: TAB NOME	{ $$ = $2; }
;

create: C db		 { creaDB($2); }
	  | create db 	 { creaDB($2); }
	  | C tab		 { creaTable($2); }
	  | create tab	 { creaTable($2); }
	  | C error 	 { yyerrok; yyclearin; printf("Per la sintassi completa usare HELP C: \n"); }
	  | create error { yyerrok; yyclearin; printf("Per la sintassi completa usare HELP C: \n"); }
;

remove: R db 		 { rimuoviDB($2); }
	  | remove db	 { rimuoviDB($2); }
	  | R tab 		 { rimuoviTable($2); }
	  | remove tab 	 { rimuoviTable($2); }
	  | R error 	 { yyerrok; yyclearin; printf("Per la sintassi completa usare HELP R: \n"); }
	  | remove error { yyerrok; yyclearin; printf("Per la sintassi completa usare HELP R: \n"); }
;

%%

main (int argc, char **argv)	{
	yyparse();	
}

yyerror( char *s){
	fprintf(stderr, "Error:%s\n", s);
}	
