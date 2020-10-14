#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

// AST building functions.

AstNode* newAst (int nodeType, AstNode* l, AstNode* r) {
	AstNode* a = malloc(sizeof(AstNode));

	if (!a) {
		yyerror("ERROR - In creating new AST NODE: Can't allocate more space.");
		exit(0);
	}

	a->nodeType = nodeType;
	a->l = l;
	a->r = r;

	return a;
}

AstNode* newRule (int nodeType, AstNode* l) {
	AstRule* a = malloc(sizeof(AstRule));

	if (!a) {
		yyerror("ERROR - In creating new AST RULE: Can't allocate more space.");
		exit(0);
	}

	a->nodeType = nodeType;
	a->l = l;

	return (AstNode*) a;
}

AstNode* newString (int nodeType, char* s) {
	AstString* a = malloc(sizeof(AstString));

	if (!a) {
		yyerror("ERROR - In creating new AST STRING: Can't allocate more space.");
		exit(0);
	}

	a->nodeType = nodeType;
	a->s = s;

	return (AstNode*) a;
}

// Recoursive routines

void run (AstNode* a) {
	switch (a->nodeType) {
		// DA DEFINIRE
		default:
			printf("Internal error: free bad node %c\n", a->nodeType);
	}
}

void drop (AstNode* a) {
	switch (a->nodeType) {
		// DA DEFINIRE
		default:
			printf("Internal error: free bad node %c\n", a->nodeType);
	}
}

// Error function

void yyerror (char* s, ...) {
	va_list ap;
	va_start(ap, s);

	fprintf(stderr, "%d: error: ", yylineno);
	vfprintf(stderr, s, ap);
	fprintf(stderr, "\n");
}