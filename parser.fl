%{
	#include "BisonP2.tab.h"
%}

%option noyywrap

TYPE("[STRINGA]"|"[INTERO]")
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
DIMENSIONE "#"[(1-9)(0-9)*]
CARICA "LOAD:"
VAR [{NOME}|{NUMERO}]+
NOME [(a-zA-Z)|"_"]+
/* DATA "("({VAR}(","{VAR})*")" */
NUMERO [0-9]
EOL \n

%%

{CREA} {return (C);}

{RIMUOVI} {return (R);}

{AGGIUNGI} {return (A);}

{DELETE} {return (D);}

{SELEZIONA} {return (S);}

{DA} {return (DA);}

{DOVE} {return (W);}

{DIMENSIONE} {yylval.str = strdup(yytext);
			  return (DIM);}

{CARICA} {return (L);}

{EOL} {return (EOL);}

{DATABASE} {return (DB);}

{TABELLA} {return (TAB);}

{NUMERO} {yylval.str = strdup(yytext);
		  return (NUM);}

{EXIT} { return (EXIT);}

{TYPE} { 	yylval.str = strdup(yytext);
			return (TYPE);}

{NOME} {yylval.str = strdup(yytext); 
		return (NOME);}

%%

