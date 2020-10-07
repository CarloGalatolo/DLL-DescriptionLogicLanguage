miniDB:	lexer.l parser.y
	bison -d parser.y
	flex lexer.l
	gcc -o $@.out DDLFunctionsDefine.c parser.tab.c lex.yy.c -lfl
