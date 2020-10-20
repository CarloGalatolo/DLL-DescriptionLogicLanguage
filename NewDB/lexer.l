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

CONGIUNZIONE (^|AND)


DISGIUNZIONE (V|OR)


COMPLEMENTO (-|COMPLEMENT)

/* Restriction definition */
ESISTENZIALE (E|EXIST)
UNIVERSALE (HAS)
INCLUSIONE (ALSO)
/* Restriction definition DA USARE IN BISON */
COMMA (.)
ASSERTIONE (:)
/* IMPLICAZIONI */
/* IMPLICAZIONE ('->'|IF) */
/* DOPPIA_IMPLICAZIONE  ('<->'|IFF) */
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