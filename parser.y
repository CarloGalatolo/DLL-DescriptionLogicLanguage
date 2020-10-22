%{	
	#include <stdio.h> 
	#include <string.h> 
	#include "TreeParser.h"	
%}

%union
{
    char *str;
}

%token <str> NAME

%token STR INT BOOL 
%token CON ROL IND 
%token CONJ DISJ CMPL CARD 
%token EXS ALL
%token DOT COLON SEMIC
%token SOPEN SCLOSE
%token WHILE IF ELSE


%error-verbose