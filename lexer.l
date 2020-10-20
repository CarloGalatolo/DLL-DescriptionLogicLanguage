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

/* Math operators */
ADD "+"
SUB "-"
MUL "*"
DIV "/"
MOD "%"

/* Comparison operators */
GETS "="
EQLS "=="
LT "<"
GT ">"
LTEQ "<="
GTEQ ">="
NOTEQ "!="

/* DEFINIZIONE CONTROLLO */
WHILE (WHILE)
IF (IF)
ELSE (ELSE)