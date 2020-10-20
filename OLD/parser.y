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

%token <str> NAME NUM TYPE DIM
%token C R A D S K DA W L DB TAB EXIT
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

LINGUAGGIO: LINGUAGGIO EOL
		  | create
		  | remove
		  | append
		  | load
;

db: DB NAME	{ $$ = $2; }
;

tab: TAB NAME { $$ = $2; }
;

key: K
   | '*'
;

sep: ','
   | '|'
;

/* Destinazione di un comando */
dest: db tab { setLoadedDB($1); $$ = $2; }
	| tab	 { $$ = $1; }
;

create: C db		 { creaDB($2); /* Loads DB as well. */ }
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

append: A dest attr { appendAttr($2, $3); }
	  | A error 	{ yyerrok;
					  printf("Il token [%s] e errato o non esistente\n",yylval);
					  yyclearin;
					  printf("Per la sintassi completa usare help A:\n"); }

load: L db { setLoadedDB($2); }

/* Attributo della tabella */
attr: NAME TYPE		  			{ myToUpper($2); sprintf($$, "%s|%s", $1, $2); }
	| NAME TYPE key	  			{ myToUpper($2); sprintf($$, "%s|%s|KEY", $1, $2); }
	| attr sep NAME TYPE		{ myToUpper($4); sprintf($$, "%s|,|%s|%s", $1, $3, $4); }
	| attr sep NAME TYPE key	{ myToUpper($4); sprintf($$, "%s|,|%s|%s|KEY", $1, $3, $4); }
;

%%

main (int argc, char **argv)	{
	yyparse();	
}

yyerror( char *s){
	fprintf(stderr, "Error:%s\n", s);
}	
