%skeleton "lalr1.cc"
%require "3.0"
%language "C++"
%defines
%locations
%error-verbose
%define api.namespace {DL}
%define parser_class_name {DL_Parser}

%code requires {
	#include "src/Onthology.hpp"
	namespace DL {
		class DL_Driver;
		class DL_Scanner;
	}
	
	// The following definitions is missing when %locations isn't used
	# ifndef YY_NULLPTR
	#  if defined __cplusplus && 201103L <= __cplusplus
	#   define YY_NULLPTR nullptr
	#  else
	#   define YY_NULLPTR 0
	#  endif
	# endif
}

%parse-param { DL_Scanner &scanner }
%parse-param { DL_Driver &driver }

%code {
	#include <iostream>
	#include <cstdlib>
	#include <fstream>
	#include <cstdio>
	#include <cstring>
	#include <string>
	#include "Tree.h"
	#include "src/dl_driver.hpp"

	#undef yylex
	#define yylex scanner.yylex
}

%define api.value.type variant	// Ignores union
%define parse.assert

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
%token INT BOOL STR CONCEPT ROLE INDV SUBS CONJ DISJ EX ALL THING NOTHING SECTION
%token END 0 "end of file"

//%type <a> EXP IstanceIndividual
%type <std::string> name
%type <DL::Individual> indv
%type <DL::Role> role
%type <DL::Concept> concept

	/* Precedence levels */
%nonassoc '?'
%left <int> COMPARISON
%nonassoc '#'
%left CONJ DISJ
%left '!'
%right '.'

%locations

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

program:
	END { std::cout << "Empty" << std::endl; } /* test */
|	names_decl END	/* test */
|	names_decl SECTION tbox END	/* test 
|	names_decl SECTION tbox SECTION abox
|	names_decl SECTION tbox SECTION abox SECTION queries*/
;

	/* FIRST SECTION */

names_decl:
	decl ';'
|	names_decl decl ';'
|	error { error(@$, std::string("In Names Declaration section")); }
;

decl:
	concept_decl
|	role_decl
|	indv_decl
;

concept_decl:
	CONCEPT NAME			{ try {DL::Onthology::getInstance().put_c($2);}
							  catch (std::logic_error e) {driver.soft_error(std::string(e.what()));} }
|	concept_decl ',' NAME	{ try {DL::Onthology::getInstance().put_c($3);}
							  catch (std::logic_error e) {driver.soft_error(std::string(e.what()));} }
;

role_decl:
	ROLE NAME			{ try {DL::Onthology::getInstance().put_r($2);}
						  catch (std::logic_error e) {driver.soft_error(std::string(e.what()));} }
|	role_decl ',' NAME	{ try {DL::Onthology::getInstance().put_r($3);}
						  catch (std::logic_error e) {driver.soft_error(std::string(e.what()));} }
;

indv_decl:
	INDV NAME			{ try {DL::Onthology::getInstance().put_i($2);}
						  catch (std::logic_error e) {driver.soft_error(std::string(e.what()));} }
|	indv_decl ',' NAME	{ try {DL::Onthology::getInstance().put_i($3);}
						  catch (std::logic_error e) {driver.soft_error(std::string(e.what()));} }
;

	/* SECOND SECTION */

tbox:
	t_stmt ';'
|	tbox t_stmt ';'
|	error { error(@$, std::string("In TBox Section")); }
;

t_stmt:
	NAME SUBS NAME	{  }
;

concept:
;

role: 
;

indv: 
;

abox:;
queries:;

	// CAPIRE SE FUNZIONA
complex_concept:	concept
				|	complex_concept CONJ concept
				|	complex_concept DISJ concept
				|	'!' complex_concept
				|	EX role '.' complex_concept
				|	ALL role '.' complex_concept
;

%%

void DL::DL_Parser::error( const location_type &l, const std::string &err_message )
{
	// Questa funzione si chiama SOLO NEL PARSER con:
	//	error(yyla.location, std::string("LA STRINGA"));
	// In caso di try/catch:
	//	error(yyla.location, std::string(e.what()));
	std::cerr << "Error: " << err_message << std::endl;
	exit(EXIT_FAILURE);
}
