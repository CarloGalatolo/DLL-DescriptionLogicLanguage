%{	
	#include <stdio.h> 
	#include <string.h> 
	#include "DDLFunctionsDefine.h"
	#include "headers/ast.h"
%}

%error-verbose

%union
{
    double d;
	char *str;
	AstNode* ast;
}

%token <d> NUM
%token <str> NAME TYPE DIM
%token C R A D S K DA W L DB TAB EXIT
%token EOL
%token VAR

%type <ast> db tab dest attr
%type <ast> program lang create remove append load

%%

program: program lang { if($1 != NULL) run($1); run($2); }
	   | program error EOL { drop($1); yyerrok; yyclearin; printf("Comando non riconosciuto, usare help per vedere la lista di comandi \n"); }
;

lang: lang EOL
	| create
	| remove
	| append
	| load
;

db: DB NAME	{ $$ = newString('B', $2); /*$$ = $2;*/ }
;

tab: TAB NAME { $$ = newString('T', $2); /*$$ = $2;*/ }
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

create: C db		 { $$ = newRule('C', $2); /*creaDB($2); /* Loads DB as well. */ }
	  | create db 	 { creaDB($2); }
	  | C tab		 { creaTable($2); }
	  | create tab	 { creaTable($2); }
;

remove: R db 		 { rimuoviDB($2); }
	  | remove db	 { rimuoviDB($2); }
	  | R tab 		 { rimuoviTable($2); }
	  | remove tab 	 { rimuoviTable($2); }
;

append: A dest attr { appendAttr($2, $3); }
;

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
