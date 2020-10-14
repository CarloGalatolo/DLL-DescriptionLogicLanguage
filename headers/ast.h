/* Lexer */
extern int yylineno;
void yyerror (char* str, ...);

// Abstract Syntax Tree nodes
typedef struct astNode {
	int nodeType;
	struct astNode *l, *r;
} AstNode;

typedef struct astRule {
	int nodeType;
	struct astNode* l;
} AstRule;
// AstRule NodeTypes:
// 'C' - create

typedef struct astString {
	int nodeType;
	char* s;
} AstString;
// AstString NodeTypes
// 'B' - database name
// 'T' - table name

/* Build the AST */
AstNode* newAst (int, AstNode*, AstNode*);
AstNode* newRule (int, AstNode*);
AstNode* newString (int, char*);

/* Run the instructions in the AST */
void run (AstNode*);

/* Drop the ASD and free the memory */
void drop (AstNode*);