%{
	#include <string>

	/* Implementation of yyFlexScanner */ 
	#include "src/dl_scanner.hpp"
	#undef  YY_DECL
	#define YY_DECL int DL::DL_Scanner::yylex( DL::DL_Parser::semantic_type * const lval, DL::DL_Parser::location_type *location )

	/* using "token" to make the returns for the tokens shorter to type */
	using token = DL::DL_Parser::token;

	/* define yyterminate as this instead of NULL */
	#define yyterminate() return( token::END )

	/* msvc2010 requires that we exclude this header file. */
	#define YY_NO_UNISTD_H

	/* update location on matching */
	#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

	int oldstate;	// Usato nei commenti.
%}

%option nodefault
%option yyclass="DL::DL_Scanner"
%option noyywrap
%option c++
%option yylineno

%x COMMENT

/* A line containing only "%%" and white spaces. */
SECTION ^[ \t]*%%[ \t]*$

%%

%{  /* Code executed at the beginning of yylex */
	yylval = lval;
	// BEGIN NAMES_DECL;
%}

	/* Changing section with "%%" */
{SECTION} { return token::SECTION; }
 /*
<NAMES_DECL>{SECTION}	{ BEGIN TBOX; return token::SECTION; }
<TBOX>{SECTION}			{ BEGIN ABOX; return token::SECTION; }
<ABOX>{SECTION}			{ BEGIN QUERY; return token::SECTION; }
 /*
<NAMES_DECL><<EOF>>	{ DL::DL_Scanner::yyerror("Unfinished names declaration before EOF."); }
<TBOX><<EOF>>		{ DL::DL_Scanner::yyerror("Unfinished TBox before EOF."); }
<ABOX><<EOF>>		{ DL::DL_Scanner::yyerror("Unfinished ABox before EOF."); }
 */
	/* Single character literal tokens */
	/* DL punctuation */
"."		{ yylval->build<std::string>(yytext); return token::DOT;}// Used in restrictions. E.g.: EX role . Concept
":" |	// Used in assertions. E.g.: indv : Concept
"," |	// Separator. Used in role assertions. E.g.: (i1, i2) : role
";" |	// End of statenìment.
"#" |	// Cardinality of a concept. E.g.: #Concept
"!" |	// Complement of a concept. E.g.: !Concept
"?" |	// Query marker. E.g.: a : C ?
	/* Brackets */
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
-?[0-9]+	{ yylval->build<int>(atoi(yytext)); return token::INTVAL; }

	/* Boolean values */
TRUE	{ yylval->build<bool>(true); return token::BOOLVAL; }
FALSE	{ yylval->build<bool>(false); return token::BOOLVAL; }

	/* Strings */
\"[^"\n]*\"	{ yylval->build<std::string>(yytext); return token::STRVAL; }
\"[^"\n]*$	{ DL::DL_Scanner::yyerror(std::string("Unterminated string ").append(yytext)); }

	/* Keywords */
ALL		{ return token::ALL; }
BOOL	{ return token::BOOL; } //?
CONCEPT	{ return token::CONCEPT; }
CONJ(UNCTION)?	{ return token::CONJ; }
DISJ(UNCTION)?	{ return token::DISJ; }
EX(ISTS)?	{ return token::EX; }
(INDIVIDUAL|INDV)	{ return token::INDV; }
INT		{ return token::INT; } //?
NOTHING	{ return token::NOTHING; }
ROLE	{ return token::ROLE; }
STR(ING)?	{ return token::STR; } //?
SUBS(UMPTION)?	{ return token::SUBS; }
THING	{ return token::THING; }

	/* Unquoted names */
[a-zA-Z_][a-zA-Z0-9_]* { yylval->build<std::string>(yytext); return token::NAME; }

	/* C-style comments */
"/*"			 { oldstate = YY_START; BEGIN COMMENT; }
<COMMENT>"*/"	 { BEGIN oldstate; }
<COMMENT>.|\n 	 ;
<COMMENT><<EOF>> { DL::DL_Scanner::yyerror("Unclosed comment before EOF."); }

	/* Ignore empty spaces and comments. */
[ \t\n] |
"//".*  ;	// C++-style comment.

%%
/*
main (int argc, char **argv) {
	yyparse();	
}

void yyerror (char *s, ...) {
	fprintf (stderr, "%s\n", s);
}
*/
