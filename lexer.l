%{
	#include "parser.tab.h"
%}

%option noyywrap case-insensitive

EXIT "EXIT:"
DATABASE "DB:"
TABELLA "TAB:"
CREA "C:"
RIMUOVI "R:"
AGGIUNGI "A:"
DELETE "D:"
SELEZIONA "S:"
DA "DA:"
DOVE "W:"
LOAD "LOAD:"

NOME [(a-zA-Z)|"_"]+
NUMERO [-+]?([0-9]*\.[0-9]+|[0-9]+\.)([Ee][-+]?[0-9]+)?
DIMENSIONE "#"[(1-9)(0-9)*]
TYPE ("[STRINGA]"|"[NUMERO]")
VAR [{NOME}|{NUMERO}]+
/* DATA "("({VAR}(","{VAR})*")" */

KEY "KEY"
COMMA ","

EOL \n

%%

{CREA} {return (C);}

{RIMUOVI} {return (R);}

{AGGIUNGI} {return (A);}

{DELETE} {return (D);}

{SELEZIONA} {return (S);}

{DA} {return (DA);}

{DOVE} {return (W);}

{LOAD} { return (L); }

{DIMENSIONE} {yylval.str = strdup(yytext);
			  return (DIM);}

{EOL} {return (EOL);}

{DATABASE} {return (DB);}

{TABELLA} {return (TAB);}

{NUMERO} {yylval.str = strdup(yytext);
		  return (NUM);}

{KEY}	{ return (KEY); }
{COMMA}	{ return (COMMA); }

{EXIT} { return (EXIT);}

{TYPE} { 	yylval.str = strdup(yytext);
			return (TYPE);}

{NOME} {yylval.str = strdup(yytext); 
		return (NOME);}

%%

