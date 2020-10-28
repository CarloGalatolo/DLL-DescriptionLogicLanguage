%{
	#include "parser.tab.h"
	#include "Tree.h"
%}

%option noyywrap

%x COMMENT

%%

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
"==" { yylval.subtok = 1; return COMPARISON; }
"<"  { yylval.subtok = 2; return COMPARISON; }
"<=" { yylval.subtok = 3; return COMPARISON; }
">"  { yylval.subtok = 4; return COMPARISON; }
">=" { yylval.subtok = 5; return COMPARISON; }
"!=" { yylval.subtok = 6; return COMPARISON; }

	/* Integer values */
-?[0-9]+	{ yylval.intval = atoi(yytext); return INTVAL; }

	/* Boolean values */
TRUE	{ yylval.intval = 1; return BOOLVAL; }
FALSE	{ yylval.intval = 0; return BOOLVAL; }

	/* Strings */
\"[^"\n]*\"	{ yylval.strval = strdup(yytext); return STRVAL; }
\"[^"\n]*$	{ yyerror("Unterminated string %s", yytext); }

	/* Keywords */
ALL		{ return ALL; }
BOOL	{ return BOOL; }
CONCEPT	{ return CONCEPT; }
CONJ(UNCTION)?	{ return CONJ; }
DISJ(UNCTION)?	{ return DISJ; }
EX(ISTS)?	{ return EX; }
INDIVIDUAL|INDV	{ return INDV; }
INT		{ return INT; }
NOTHING	{ return NOTHING; }
ROLE	{ return ROLE; }
STR(ING)?	{ return STR; }
SUBS(UMPTION)?	{ return SUBS; }
THING	{ return THING; }

	// Unquoted names
[a-zA-Z_][a-zA-Z0-9_]* { yylval.strval = strdup(yytext); return NAME; }

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
	fprintf (stderr, "%s\n", s);
}