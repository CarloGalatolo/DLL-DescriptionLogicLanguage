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
# include "TreeParser.h"



extern int yylineno; /* from lexer */
void yyerror(char *s, ...);



char ** individual;
int individualList=0;

struct ast *chainAst(int nodetype, struct ast *l, struct ast *r)
{
 struct ast *a = malloc(sizeof(struct ast));

 if(!a) {

 exit(0);
 }
 a->nodetype = nodetype;
 a->l = l;
 a->r = r;
 return a;
}

struct ast *astComplexType(int nodetype, char* namelist)
{
 struct stringval *a = malloc(sizeof(struct stringval));
 if(!a) {

 exit(0);
 }
 a->nodetype = nodetype;
 a->string = namelist;
 return (struct ast *)a;
}


double eval(struct ast *a)
{
 char* s;
 double v;
 switch(a->nodetype) {
 case 'I': 	 
 			 s= ((struct stringval *)a)->string; 
			 v=listIndividual(s); 
			 break;
 case 'H': v = eval(a->l); eval(a->r); break;
 default: printf("internal error: bad node %c\n", a->nodetype);
 }
 return v;
}



int listIndividual(char* individualList){
	char* token = strtok(individualList, "|");
			while (token) {
					// Nome dell'attributo					
					//	strcpy(individual[individualList], token);
					printf(token);
					individualList++;
					token = strtok(0, "|");
			}
			return 1;
}



