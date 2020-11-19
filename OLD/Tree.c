# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
# include "Tree.h"


int counter=0;

struct ast *newast(int nodetype, struct ast *l, struct ast *r){
struct ast *a = malloc(sizeof(struct ast));

 if(!a) {
 yyerror("out of space");
 exit(0);
 }
 a->nodetype = nodetype;
 a->l = l;
 a->r = r;
 return a;
}

struct ast *newnum(char* d){
 struct numval *a = malloc(sizeof(struct numval));
 if(!a) {
 yyerror("out of space");
 exit(0);
 }
 a->nodetype = 'I';
 a->number = d;
 return (struct ast *)a;
}


double eval(struct ast *a){
 double v; 
 char* t;
 switch(a->nodetype) {
 case 'I': t = ((struct numval *)a)->number;printf("[%s]\n",t); listIndividual(t); break;
 case 'H': v = eval(a->l) + eval(a->r); break;
 default: printf("internal error: bad node %c\n", a->nodetype);
 }
 return v;
}

int listIndividual(char* individualList){
			char* token = (char*) malloc(100);
			printf("(%s) \n",individualList);
			token = strtok(individualList, "|");
			while (token) {
					// Nome dell'attributo					
					//	strcpy(individual[individualList], token);
					printf(token);
					counter++;
					token = strtok(0, "|");
			}
			return 1;
}
