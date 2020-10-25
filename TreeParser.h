#define HASHSIZE 101
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

extern int yylineno; /* from lexer */
void yyerror(char *s, ...);

char ** individual;
int individualList;

struct ast {
 int nodetype;
 struct ast *l;
 struct ast *r;
};

struct stringval {
 char* string; /* type K for constant */
 int nodetype; 
};


struct ast *chainAst(int nodetype, struct ast *l, struct ast *r);

struct ast *astComplexType(int nodetype, char* namelist);


double eval(struct ast *a);

int listIndividual(char* individualList);
