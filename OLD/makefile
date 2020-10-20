miniDB:	lexer.l parser.y DDLFunctionsDefine.h
	bison -d parser.y
	flex lexer.l
	gcc -o $@.out DDLFunctionsDefine.c parser.tab.c lex.yy.c -lfl
