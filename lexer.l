%{
	#include "parser.tab.h"
	#include "Tree.h"
%}

%option noyywrap

%x COMMENT

/* Simple Types */
INT  ("INT")  /* Integer */
BOOL ("BOOL") /* Boolean */
STR  ("STR")  /* String  */

/* Complex Types */
CONCETTO (CONCEPT)
RUOLO (ROLE)
INDIVIDUO (INDIVIUAL)

/* Logic operators */
CONJ ("&&")	/* Conjunction */
DISJ ("||") /* Disjunction */

/* Restrictions */
ESISTENZIALE EX(ISTS)?
UNIVERSALE   (ALL)

/*ISTANCE_ASSERTION meglio gestirle in Bison */
/*CONCEPT_ASSERTION definisco pero le parentesi per la struttura, con l'aggiunta del terminatore */
STRUCTURE_FLAG_START ('(')
STRUCTURE_FLAG_END (')')

/*DEFINIZIONE CONTROLLO */
WHILE (WHILE)
IF (IF)
ELSE (ELSE)

%%

	/* Single character literal tokens */
"=" |	// Assingment
	// DL punctuation
"." |	// Used in restrictions. E.g.: EX role . Concept
":" |	// Used in assertions. E.g.: indv : Concept
"|" |	// Concat more assertions of the same concept or role. E.g.: i1 | i2 : Concept
"," |	// Separator used in role assertions. E.g.: (i1, i2) : role
";" |	// End of statenìment.
"#" |	// Cardinality of a concept. E.g.: #Concept
"!" |	// Complement of a concept. E.g.: !Concept. Also factorial operator for integers.
	// Math operators
"+" |
"-" |
"*" |
"/" |
"^" |	// Power. E.g.: 2^3 (= 8)
"%" |	// Modulo. E.g.: 5%2 (= 1)
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

{STR}  { return (STR); }
{INT}  { return (INT); }
{BOOL} { return (BOOL); }

	/* Integer values */
-?[0-9]+	{ yylval.intval = atoi(yytext); return INTVAL; }

	/* Boolean values */
TRUE	{ yylval.intval = 1; return BOOLVAL; }
FALSE	{ yylval.intval = 0; return BOOLVAL; }

	/* Strings */
\"[^"\n]*\"	{ yylval.strval = strdup(yytext); return STRVAL; }
\"[^"\n]*$	{ yyerror("Unterminated string %s", yytext); }

{CONCETTO}  { return (CON); }
{RUOLO}	    { return (ROL); }
{INDIVIDUO} { return (IND); }

{CONJ} {return (CONJ);}
{DISJ} {return (DISJ);}

{ESISTENZIALE} {return (EXS)}
{UNIVERSALE} {return (ALL);}

{STRUCTURE_FLAG_START} {return (SOPEN);}
{STRUCTURE_FLAG_END} {return (SCLOSE);}

{WHILE} {return (WHILE);}
{IF}    {return (IF);}
{ELSE}  {return (ELSE);}

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