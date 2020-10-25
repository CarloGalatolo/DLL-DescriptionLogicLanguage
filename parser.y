%{	
	#include <stdio.h> 
	#include <string.h> 
	#include "Tree.h"	
%}

%union {
 struct ast *a;
 char *str;
}

%token <str> NAME

%token STR INT BOOL 
%token CON ROL IND 
%token CONJ DISJ CMPL CARD 
%token EXS ALL
%token DOT COLON SEMIC COMMA
%token SOPEN SCLOSE
%token WHILE IF ELSE

%type <a> EXP IstanceIndividual
%type <str> name

%%

LINGUAGGIO: EXP {eval($1);}
;

EXP: EXP IstanceIndividual { $$=newast('H',$1,$2);}
|IstanceIndividual {$$=$1;}
;

IstanceIndividual: IND name SEMIC { printf("%s",$2);
								$$=newnum($2);}
;

name: name COMMA NAME {sprintf($$, "%s|%s", $1,$3);}
| NAME { sprintf($$, "%s", $1);}
;