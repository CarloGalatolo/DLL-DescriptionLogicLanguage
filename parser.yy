%skeleton "lalr1.cc"
%require "3.0"
%language "C++"
%defines
%error-verbose
%define api.namespace {DL}
%define parser_class_name {DL_Parser}

%code requires {
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
	#include "src/dl_driver.hpp"
	#include "src/Onthology.hpp"

	#undef yylex
	#define yylex scanner.yylex
}

%define api.value.type variant	// Ignores union
%define parse.assert

%token <int> BOOLVAL INTVAL
%token <std::string> NAME STRVAL
%token INT BOOL STR CONCEPT ROLE INDV SUBS CONJ DISJ EX ALL THING NOTHING SECTION
%token END 0 "end of file"

//%type <a> EXP IstanceIndividual
%type <std::string> complex_concept indv_assert role_assert
%type <DL::Individual> indv
%type <DL::Role> role
//%type <DL::Concept> 

	/* Precedence levels */
%nonassoc '?'
%left <int> COMPARISON
%nonassoc '#'
%nonassoc CONJ DISJ
%left '!'
%nonassoc '.'

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
	END { std::cout << "Input file empty." << std::endl; }
|	names_decl END	/* debug */
|	names_decl SECTION abox END	/* debug */ 
|	names_decl SECTION abox SECTION tbox END /* debug
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

abox:
	a_stmt ';'
|	abox a_stmt ';'
|	error { error(@$, std::string("In ABox Section")); }
;

a_stmt:
	indv_assert
|	role_assert
;

indv_assert:
	NAME ':' NAME		 { try {DL::Onthology::getInstance().get_c($3).addIndividual($1);}
							catch (std::logic_error e) {driver.soft_error(std::string(e.what()));}
							$$ = $3; }
|	NAME ',' indv_assert { try {DL::Onthology::getInstance().get_c($3).addIndividual($1);}
							catch (std::logic_error e) {driver.soft_error(std::string(e.what()));}
							$$ = $3; }
;

role_assert:
	'(' NAME ',' NAME ')' ':' NAME			{ try {DL::Onthology::getInstance().get_r($7).insert($2, $4);}
											  catch (std::logic_error e) {driver.soft_error(std::string(e.what()));}
											  $$ = $7; }
|	'(' NAME ',' NAME ')' ',' role_assert	{ try {DL::Onthology::getInstance().get_r($7).insert($2, $4);}
											  catch (std::logic_error e) {driver.soft_error(std::string(e.what()));}
											  $$ = $7; }

	/* THIRD SECTION */

tbox:
	t_stmt ';'
|	tbox t_stmt ';'
|	error { error(@$, std::string("In TBox Section")); }
;

t_stmt:
	NAME SUBS NAME	{ DL::Onthology::getInstance().subsumption($1, $3); }
|	complex_concept SUBS NAME { DL::Onthology::getInstance().subsumption($1, $3); }
|	complex_concept
;

complex_concept:
	NAME CONJ NAME { $$ = DL::Onthology::getInstance().conjunction($1, $3); }
|	NAME DISJ NAME { $$ = DL::Onthology::getInstance().disjunction($1, $3); }
|	'!' NAME { $$ = DL::Onthology::getInstance().negation($2); }
//|	'(' '!' NAME ')'	{ $$ = DL::Onthology::getInstance().negation($3); }
|	'!' complex_concept	{ $$ = DL::Onthology::getInstance().negation($2); }
/*|	complex_concept CONJ concept
|	complex_concept DISJ concept
|	EX role '.' complex_concept
|	ALL role '.' complex_concept 
*/
;

queries:;



%%

void DL::DL_Parser::error( const location_type &l, const std::string &err_message )
{
	// Questa funzione si chiama SOLO NEL PARSER con:
	//	error(yyla.location, std::string("LA STRINGA"));
	// In caso di try/catch:
	//	error(yyla.location, std::string(e.what()));
	std::cerr << "Error: " << err_message << " at " << l << std::endl;
	exit(EXIT_FAILURE);
}
