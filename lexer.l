%{
	#include "parser.tab.h"
%}

%option noyywrap case-insensitive

/* Type definition */

STRINGA (S|STRING)
INTERO (I|INTEGER)
BOOLEANO (B|BOOLEAN)


/* Compound Type definition */

CONCETTO (CONCEPT|C)
RUOLO (ROLE|R)
INDIVIDUO (INDIVIUAL|CHAP|I)

/* Logic operators definition */
INTERSEZIONE (=|INTESECTION)
CONGIUNZIONE (^|AND)

UNIONE (U|UNION)
DISGIUNZIONE (V|OR)

NEGAZIONE (!|NOT)
COMPLEMENTO (-|COMPLEMENT)

/* Restriction definition */
ESISTENZIALE (E|EXIST)
UNIVERSALE (A|HAS|IS)
/* Restriction definition DA USARE IN BISON */
COMMA (.)
/* IMPLICAZIONI */
IMPLICAZIONE ('->'|IF)
DOPPIA_IMPLICAZIONE  ('<->'|IFF)
/* Structure for assertion */

/*ISTANCE_ASSERTION meglio gestirle in Bison */
/*CONCEPT_ASSERTION definisco pero le parentesi per la struttura, con l'aggiunta del terminatore */
STRUCTURE_FLAG_START ('(')
STRUCTURE_FLAG_END (')')
STRUCTURE_TERMINATOR(';')

/*DEFINIZIONE CONTROLLO */
WHILE (W|WHILE)
IF (IF)
ELSE (ELSE)