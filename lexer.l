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
CMPL ("!")  /* Complement  */
CARD "#"	/* Cardinality */

/* Restrictions */
EXISTENTIAL (EXIST)
UNIVERSAL   (ALL)

/* Punctuation */
DOT   (".") /* Used in restrictions */
COLON (":") /* Used in assertions */
SEMIC (";") /* End of statement */
COMMA   (",") /* Separator */

/*ISTANCE_ASSERTION meglio gestirle in Bison */
/*CONCEPT_ASSERTION definisco pero le parentesi per la struttura, con l'aggiunta del terminatore */
STRUCTURE_FLAG_START ('(')
STRUCTURE_FLAG_END (')')

/*DEFINIZIONE CONTROLLO */
WHILE (W|WHILE)
IF (IF)
ELSE (ELSE)

NAME [(a-zA-Z)|"_"]+



%%

	/* Single character literal tokens */
"*" |
"," |
"|" 	{ return yytext[0]; }

{STR} { return (STR); }
{INT} { return (INT); }
{BOOL} { return (BOOL); }


{CONCETTO}	 { return (CON); }
{RUOLO}	 { return (ROL); }
{INDIVIDUO} {return (IND);}

{CONJ} {return (CONJ);}
{DISJ} {return (DISJ);}
{CMPL} {return (CMPL);}
{CARD} {return (CARD);}

{ESISTENZIALE} {return (EXS)}
{UNIVERSALE} {return (ALL);}


{DOT} {return (DOT);}
{COLON} {return (COLON);}
{SEMIC} {return (SEMIC);}
{COMMA}  {return (COMMA);}

{STRUCTURE_FLAG_START} {return (SOPEN);}
{STRUCTURE_FLAG_END} {return (SCLOSE);}

{WHILE} {return (WHILE);}
{IF} {return (IF);}
{ELSE} {return (ELSE);}

{NAME} {yylval.str = strdup(yytext); 
		return (NAME);}

%%