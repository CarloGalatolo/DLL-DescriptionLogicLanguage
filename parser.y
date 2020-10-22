%{	
	#include <stdio.h> 
	#include <string.h> 
	#include "TreeParser.h"	
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

%type <a> Exp IstanceIndividual
%type name

%error-verbose 

%%

Linguaggio: Linguaggio Exp EOL{eval($2);cleartree($2);}
|Linguaggio EOL
;

Exp: exp IstanceIndividual { $$=chainAst("CH",$1,$2);}
| exp IstanceRole
| exp IstanceConcept
;

IstanceIndividual: IND SOPEN name SCLOSE { $$=astIndividual("IND",$3);}
;

name: name COMMA NAME {sprintf($$, "%s|%s", $1,$3);}
| NAME { sprintf($$, "%s", $1);}
;