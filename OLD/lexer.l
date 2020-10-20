%{
	#include "parser.tab.h"
%}

%option noyywrap case-insensitive

CREATE (C|CREATE)":"
REMOVE (R|REMOVE)":"
APPEND (A|APPEND)":"
LOAD   (L|LOAD)":"

KEY (K|KEY)

EXIT "EXIT:"
DATABASE "DB:"
TABELLA "TAB:"
DELETE "D:"
SELEZIONA "S:"
DA "DA:"
DOVE "W:"

NAME [(a-zA-Z)|"_"]+
NUMERO [-+]?([0-9]*\.[0-9]+|[0-9]+\.)(E[-+]?[0-9]+)?
DIMENSIONE "#"[(1-9)(0-9)*]
TYPE ("[STRINGA]"|"[NUMERO]")
VAR [{NAME}|{NUMERO}]+
/* DATA "("({VAR}(","{VAR})*")" */

EOL \n

%%

	/* Single character literal tokens */
"*" |
"," |
"|" 	{ return yytext[0]; }

{CREATE} { return (C); }
{REMOVE} { return (R); }
{APPEND} { return (A); }
{LOAD}	 { return (L); }

{KEY}	 { return (K); }

{DELETE} {return (D);}

{SELEZIONA} {return (S);}

{DA} {return (DA);}

{DOVE} {return (W);}



{DIMENSIONE} {yylval.str = strdup(yytext);
			  return (DIM);}

{EOL} {return (EOL);}

{DATABASE} {return (DB);}

{TABELLA} {return (TAB);}

{NUMERO} {yylval.str = strdup(yytext);
		  return (NUM);}

{EXIT} { return (EXIT);}

{TYPE} { 	yylval.str = strdup(yytext);
			return (TYPE);}

{NAME} {yylval.str = strdup(yytext); 
		return (NAME);}

%%

