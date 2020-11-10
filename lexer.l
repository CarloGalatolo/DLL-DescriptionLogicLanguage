%{
	#include <string>
	#include "parser.tab.hh"
	#include "Tree.h"

	/* Implementation of yyFlexScanner */ 
	#include "mc_scanner.hpp"
	#undef  YY_DECL
	#define YY_DECL int MC::MC_Scanner::yylex( MC::MC_Parser::semantic_type * const lval, MC::MC_Parser::location_type *location )

	/* using "token" to make the returns for the tokens shorter to type */
	using token = MC::MC_Parser::token;

	/* define yyterminate as this instead of NULL */
	#define yyterminate() return( token::END )

	/* msvc2010 requires that we exclude this header file. */
	#define YY_NO_UNISTD_H

	/* update location on matching */
	#define YY_USER_ACTION loc->step(); loc->columns(yyleng);
%}

%option debug
%option nodefault
%option yyclass="MC::MC_Scanner"
%option noyywrap
%option c++

/* Start states */
%s NAMES_DECL
%s TBOX
%s ABOX
%s INT_D
%s QUERY
%x COMMENT

/* A line containing only "%%" and white spaces. */
SECTION ^[ \t]*%%[ \t]*$

%%

%{  /* Code executed at the beginning of yylex */
	yylval = lval;
	initial_state = YY_START;
%}

	/* Changing section with "%%" */
<NAMES_DECL>{SECTION}	{ BEGIN TBOX; }
<TBOX>{SECTION}			{ BEGIN ABOX; }
<ABOX>{SECTION}			{ BEGIN INT_D; }
<INT_D>{SECTION}		{ BEGIN QUERY; }

	/* Single character literal tokens */
	// DL punctuation
"." |	// Used in restrictions. E.g.: EX role . Concept
":" |	// Used in assertions. E.g.: indv : Concept
"," |	// Separator used in role assertions. E.g.: (i1, i2) : role
";" |	// End of statenìment.
"#" |	// Cardinality of a concept. E.g.: #Concept
"!" |	// Complement of a concept. E.g.: !Concept.
	// Brackets
"(" |
")" |
"{" |	// Curly brackets determine a block of statements or instructions.
"}"		{ return yytext[0]; }

	/* Comparison operators */
"==" { yylval->build<int>(1); return token::COMPARISON; }
"<"  { yylval->build<int>(2); return token::COMPARISON; }
"<=" { yylval->build<int>(3); return token::COMPARISON; }
">"  { yylval->build<int>(4); return token::COMPARISON; }
">=" { yylval->build<int>(5); return token::COMPARISON; }
"!=" { yylval->build<int>(6); return token::COMPARISON; }

	/* Integer values */
-?[0-9]+	{ yylval->build<int>(atoi(yytext)); return INTVAL; }

	/* Boolean values */
TRUE	{ yylval->build<int>(1); return token::BOOLVAL; }
FALSE	{ yylval->build<int>(0); return token::BOOLVAL; }

	/* Strings */
\"[^"\n]*\"	{ yylval->build<std::string>(yytext); return token::STRVAL; }
\"[^"\n]*$	{ yyerror("Unterminated string %s", yytext); }

	/* Keywords */
ALL		{ return token::ALL; }
BOOL	{ return token::BOOL; }
CONCEPT	{ return token::CONCEPT; }
CONJ(UNCTION)?	{ return token::CONJ; }
DISJ(UNCTION)?	{ return token::DISJ; }
EX(ISTS)?	{ return token::EX; }
INDIVIDUAL|INDV	{ return token::INDV; }
INT		{ return token::INT; }
NOTHING	{ return token::NOTHING; }
ROLE	{ return token::ROLE; }
STR(ING)?	{ return token::STR; }
SUBS(UMPTION)?	{ return token::SUBS; }
THING	{ return token::THING; }

	// Unquoted names
[a-zA-Z_][a-zA-Z0-9_]* { yylval.strval = strdup(yytext); return token::NAME; }

	// C-style comments
"/*"			 { oldstate = YY_START; BEGIN COMMENT; }
<COMMENT>"*/"	 { BEGIN oldstate; }
<COMMENT>.|\n 	 ;
<COMMENT><<EOF>> { yyerror("Unclosed comment before EOF."); }

	// Ignore empty spaces and comments.
[ \t\n] |
"//".*  ;	// C++-style comment.

%%

main (int argc, char **argv) {
	yyparse();	
}

void yyerror (char *s, ...) {
	/*fprintf (stderr, "%s\n", s);*/

	std::cerr << s << std::endl;
}