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
	#include "src/Ontology.hpp"

	#undef yylex
	#define yylex scanner.yylex
}

%define api.value.type variant	// Ignores union
%define parse.assert

%token <std::string> NAME
%token DOT CONCEPT ROLE INDV SUBS COINCIDENCE CONJ DISJ EX ALL SECTION
%token END 0 "end of file"

%type <std::string> complex_concept indv_assert role_assert

	/* Precedence levels */
%left CONJ DISJ
%right '!'
%nonassoc DOT

%locations

%%

program:
	names_decl SECTION abox SECTION tbox {DL::Ontology::getInstance().validate();} END
;

	/* FIRST SECTION */

names_decl:
	/* empty */
|	names_decl decl ';'
;

decl:
	concept_decl
|	role_decl
|	indv_decl
;

concept_decl:
	CONCEPT NAME			{ try {DL::Ontology::getInstance().put_c($2);}
							  catch (std::logic_error e) {driver.soft_error(std::string(e.what()));} }
|	concept_decl ',' NAME	{ try {DL::Ontology::getInstance().put_c($3);}
							  catch (std::logic_error e) {driver.soft_error(std::string(e.what()));} }
;

role_decl:
	ROLE NAME			{ try {DL::Ontology::getInstance().put_r($2);}
						  catch (std::logic_error e) {driver.soft_error(std::string(e.what()));} }
|	role_decl ',' NAME	{ try {DL::Ontology::getInstance().put_r($3);}
						  catch (std::logic_error e) {driver.soft_error(std::string(e.what()));} }
;

indv_decl:
	INDV NAME			{ try {DL::Ontology::getInstance().put_i($2);}
						  catch (std::logic_error e) {driver.soft_error(std::string(e.what()));} }
|	indv_decl ',' NAME	{ try {DL::Ontology::getInstance().put_i($3);}
						  catch (std::logic_error e) {driver.soft_error(std::string(e.what()));} }
;

	/* SECOND SECTION */

abox:
	/* empty */	
|	abox a_stmt ';'
;

a_stmt:
	indv_assert
|	role_assert
;

indv_assert:
	NAME ':' NAME		  { try {DL::Ontology::getInstance().get_c($3).addIndividual($1);}
							catch (std::logic_error e) {driver.soft_error(std::string(e.what()));}
							$$ = $3; }
|	NAME ',' indv_assert  { try {DL::Ontology::getInstance().get_c($3).addIndividual($1);}
							catch (std::logic_error e) {driver.soft_error(std::string(e.what()));}
							$$ = $3; }
;

role_assert:
	'(' NAME ',' NAME ')' ':' NAME			{ try {DL::Ontology::getInstance().get_r($7).insert($2, $4);}
											  catch (std::logic_error e) {driver.soft_error(std::string(e.what()));}
											  $$ = $7; }
|	'(' NAME ',' NAME ')' ',' role_assert	{ try {DL::Ontology::getInstance().get_r($7).insert($2, $4);}
											  catch (std::logic_error e) {driver.soft_error(std::string(e.what()));}
											  $$ = $7; }

	/* THIRD SECTION */

tbox:
	/* empty */
|	tbox t_stmt ';'
;

t_stmt:
	NAME 					SUBS NAME					 { DL::Ontology::getInstance().subsumption($1, $3); }
|	complex_concept 		SUBS NAME					 { DL::Ontology::getInstance().subsumption($1, $3); }
|	'(' complex_concept ')' SUBS NAME 					 { DL::Ontology::getInstance().subsumption($2, $5); }
|	complex_concept			SUBS complex_concept		 { DL::Ontology::getInstance().subsumption($1, $3); }
|	complex_concept			SUBS '(' complex_concept ')' { DL::Ontology::getInstance().subsumption($1, $4); }
|	'(' complex_concept ')'	SUBS complex_concept		 { DL::Ontology::getInstance().subsumption($2, $5); }
|	'(' complex_concept ')' SUBS '(' complex_concept ')' { DL::Ontology::getInstance().subsumption($2, $6); }
|	NAME 					'=' NAME					 { DL::Ontology::getInstance().alias($1, $3); }
|	complex_concept 		'=' NAME					 { DL::Ontology::getInstance().alias($1, $3); }
|	'(' complex_concept ')' '=' NAME 					 { DL::Ontology::getInstance().alias($2, $5); }
|	NAME 					COINCIDENCE NAME			 { DL::Ontology::getInstance().coincidence($1, $3); }
|	complex_concept 		COINCIDENCE NAME			 { DL::Ontology::getInstance().coincidence($1, $3); }
|	'(' complex_concept ')' COINCIDENCE NAME			 { DL::Ontology::getInstance().coincidence($2, $5); }
|	complex_concept										 /* for testing purposes */
;

complex_concept:
	NAME 					CONJ NAME 					 { $$ = DL::Ontology::getInstance().conjunction($1, $3); }
|	'(' complex_concept ')' CONJ NAME    				 { $$ = DL::Ontology::getInstance().conjunction($2, $5); }
|	'(' complex_concept ')' CONJ '(' complex_concept ')' { $$ = DL::Ontology::getInstance().conjunction($2, $6); }
|	NAME 					DISJ NAME 					 { $$ = DL::Ontology::getInstance().disjunction($1, $3); }
|	'(' complex_concept ')' DISJ NAME    				 { $$ = DL::Ontology::getInstance().disjunction($2, $5); }
|	'(' complex_concept ')' DISJ '(' complex_concept ')' { $$ = DL::Ontology::getInstance().disjunction($2, $6); }
|	'!' NAME 					 						 { $$ = DL::Ontology::getInstance().negation($2,false); }
|	'!' '(' complex_concept ')'	 						 { $$ = DL::Ontology::getInstance().negation($3,true); }
|	EX  NAME DOT NAME 									 { $$ = DL::Ontology::getInstance().existential($2, $4); }
|	EX  NAME DOT '(' complex_concept ')'				 { $$ = DL::Ontology::getInstance().existential($2, $5); }
|	ALL NAME DOT NAME 									 { $$ = DL::Ontology::getInstance().universal($2, $4); }
|	ALL NAME DOT '(' complex_concept ')'				 { $$ = DL::Ontology::getInstance().universal($2, $5); }
;

%%

void DL::DL_Parser::error( const location_type &l, const std::string &err_message )
{
	/**
	 * Funzione errore del parser. Va chiamata così:
	 * 		error(@N, std::string("qualcosa"));
	 * In caso di try/catch:
	 *		error(@N, std::string(e.what()));
	 */
	std::cerr << "Error: " << err_message << " at " << l << std::endl;
	exit(EXIT_FAILURE);
}