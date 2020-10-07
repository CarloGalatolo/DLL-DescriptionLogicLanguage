miniDB:	parser.fl BisonP2.y
	bison -d BisonP2.y
	flex parser.fl
	gcc -o $@.out DDLFunctionsDefine.c BisonP2.tab.c lex.yy.c -lfl
