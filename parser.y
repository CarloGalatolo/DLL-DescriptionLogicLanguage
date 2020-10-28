%require "3.2"
%language "c++"

%{	
	#include <stdio.h> 
	#include <string.h> 
	#include "Tree.h"	
%}

%union {
	int intval;
	char* strval;
	int subtok;
	struct ast *a;
}

%token <intval> BOOLVAL INTVAL
%token <strval> NAME STRVAL
%token INT BOOL STR CONCEPT ROLE INDV SUBS CONJ DISJ EX ALL THING NOTHING

%type <a> EXP IstanceIndividual
%type <strval> name

	/* Precedence levels */
%right '.' ':'
%left <subtok> COMPARISON
%nonassoc '#'
%left CONJ
%left DISJ
%left '!'

%%

LINGUAGGIO: EXP { eval($1); }
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

concept:	CONCEPT NAME
		|	THING
		|	NOTHING
;

role:	ROLE NAME
;

indv: INDV NAME
;

	// CAPIRE SE FUNZIONA
complex_concept:	concept
				|	concept CONJ complex_concept
				|	concept DISJ complex_concept
				|	'!' complex_concept
				|	EX role '.' complex_concept
				|	ALL role '.' complex_concept
;
