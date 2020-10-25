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
INDIVIDUO (INDIVIUAL)

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

NAME [(a-zA-Z)|"_"]+

%%
{INT} { return (INT); }
{STR} { return (STR); }
{BOOL} { return (BOOL); }


{CONCETTO}	 { return (CON); }
{RUOLO}	 { return (ROL); }
{INDIVIDUO} {return (IND);}

{CONJ} {return (CONJ);}
{DISJ} {return (DISJ);}
{CMPL} {return (CMPL);}
{CARD} {return (CARD);}

{EXISTENTIAL} {return (EXS);}
{UNIVERSAL} {return (ALL);}


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

main (int argc, char **argv)	{
	yyparse();	
}

 void yyerror (char *s, ...) {
   fprintf (stderr, "%s\n", s);
 }