%skeleton "lalr1.cc"
%require "3.0"
//%language "c++"
%debug 
%defines 
%define api.namespace {MC}
%define api.value.type variant	// Ignores union
%define parser_class_name {MC_Parser}

%{
	#include <iostream>
	#include <cstdlib>
	#include <fstream>
	#include <cstdio>
	#include <cstring>
	#include <string>
	#include "Tree.h"
%}

%code requires {
	namespace MC {
		class MC_Scanner;
	}
	
	#include "src/Onthology.hpp"

	// The following definitions is missing when %locations isn't used
	# ifndef YY_NULLPTR
	#  if defined __cplusplus && 201103L <= __cplusplus
	#   define YY_NULLPTR nullptr
	#  else
	#   define YY_NULLPTR 0
	#  endif
	# endif
}

%parse-param { MC_Scanner  &scanner }

%code {
	#undef yylex
	#define yylex scanner.yylex
}

/*
%union {
	int intval;
	char* strval;
	int subtok;
	struct ast *a;
}
*/
%token <int> BOOLVAL INTVAL
%token <std::string> NAME STRVAL
%token INT BOOL STR CONCEPT ROLE INDV SUBS CONJ DISJ EX ALL THING NOTHING

//%type <a> EXP IstanceIndividual
%type <std::string> name
%type <DL::Individual> indv
%type <DL::Role> role
%type <DL::Concept> concept

	/* Precedence levels */
%right '.'
%left <int> COMPARISON
%nonassoc '#'
%left CONJ DISJ
%left '!'

%%
/*
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
*/
concept:	CONCEPT NAME	{ DL::Onthology.AllConcepts.push_back(new Concept($2)); }
		|	THING
		|	NOTHING
;

role: ROLE NAME	{ DL::Onthology.AllRoles.push_back(new Role($2)); }
;

indv: INDV NAME	{ DL::Onthology.AllIndividuals.push_back(new Individual($2)); }
;

	// CAPIRE SE FUNZIONA
complex_concept:	concept
				|	complex_concept CONJ concept
				|	complex_concept DISJ concept
				|	'!' complex_concept
				|	EX role '.' complex_concept
				|	ALL role '.' complex_concept
;
