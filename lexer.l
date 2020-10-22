%{
	#include "parser.tab.h"
%}

%option noyywrap

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
ESISTENZIALE (EXISTS)
UNIVERSALE   (ALL)

/*ISTANCE_ASSERTION meglio gestirle in Bison */
/*CONCEPT_ASSERTION definisco pero le parentesi per la struttura, con l'aggiunta del terminatore */
STRUCTURE_FLAG_START ('(')
STRUCTURE_FLAG_END (')')

/*DEFINIZIONE CONTROLLO */
WHILE (W|WHILE)
IF (IF)
ELSE (ELSE)

NAME ([a-zA-Z]|[_-])+

%%
	// Ignore empty spaces and comments.
" "    |
"\t"   |
"\n"   |
"//".* |	// C++-style comment.
"/*".*"*/" {} // C-style comment.

	/* Single character literal tokens */
	// DL punctuation
"." |	// Used in restrictions. E.g.: EXISTS role . Concept
":" |	// Used in assertions. E.g.: indv : Concept
"|" |	// Concat more assertions of the same concept or role. E.g.: i1 | i2 : Concept
"," |	// Separator used in role assertions. E.g.: (i1, i2) : role
";" |	// End of statenìment.
"#" |	// Cardinality of a concept. E.g.: #Concept
"!" |	// Complement of a concept. E.g.: !Concept
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

{STR}  { return (STR); }
{INT}  { return (INT); }
{BOOL} { return (BOOL); }

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

{NAME} { yylval.str = strdup(yytext); return (NAME);}

%%