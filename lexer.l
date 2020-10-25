%{
	#include "parser.tab.h"
	#include "Tree.h"
%}

%option noyywrap

INT  (INT)  
BOOL (BOOL) 
STR  (STR)  

CONCETTO (CONCEPT)
RUOLO (ROLE)
INDIVIDUO (INDIVIDUAL)

CONJ (&&)	
DISJ ("||") 
CMPL ("!") 
CARD ("#")	

EXISTENTIAL (EXIST)
UNIVERSAL   (ALL)

DOT   (".") 
COLON (":") 
SEMIC (";") 
COMMA   (",") 

STRUCTURE_FLAG_START ("(")
STRUCTURE_FLAG_END (")")

WHILE (W|WHILE)
IF (IF)
ELSE (ELSE)

NAME ([a-zA-Z]|[_-])+

%%

{STR}  { return (STR); }
{INT}  { return (INT); }
{BOOL} { return (BOOL); }

{CONCETTO}  { return (CON); }
{RUOLO}	    { return (ROL); }
{INDIVIDUO} { return (IND); }

{CONJ} {return (CONJ);}
{DISJ} {return (DISJ);}

{EXISTENTIAL} {return (EXS);}
{UNIVERSAL} {return (ALL);}

{DOT}  { return (DOT); }
{COLON}	    { return (COLON); }
{SEMIC} { return (SEMIC); }
{COMMA} { return (COMMA); }

{STRUCTURE_FLAG_START} {return (SOPEN);}
{STRUCTURE_FLAG_END} {return (SCLOSE);}

{WHILE} {return (WHILE);}
{IF}    {return (IF);}
{ELSE}  {return (ELSE);}

{NAME} { yylval.str = strdup(yytext); return (NAME);}

%%

main (int argc, char **argv)	{
	yyparse();	
}

 void yyerror (char *s, ...) {
   fprintf (stderr, "%s\n", s);
 }