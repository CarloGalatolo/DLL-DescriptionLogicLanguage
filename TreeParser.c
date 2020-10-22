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


struct ast *
chainAst(int nodetype, struct ast *l, struct ast *r)
{
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

struct ast *
astComplexType(int nodetype, string namelist)
{
 struct stringval *a = malloc(sizeof(struct stringval));
 if(!a) {
 yyerror("out of space");
 exit(0);
 }
 a->nodetype = nodetype;
 a->string = namelist;
 return (struct ast *)a;
}



double eval(struct ast *a)
{
 double v; calculated value of this subtree
 switch(a->nodetype) {

 case 'IND': v=listIndividual(a->string) ; break;
 case 'H': v = eval(a->l); eval(a->r); break;

 default: printf("internal error: bad node %c\n", a->nodetype);
 }
 return v;
}

