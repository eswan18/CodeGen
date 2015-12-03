all: cminor

cminor: token.o lex.yy.o main.o parser.tab.o expr.o decl.o stmt.o type.o param_list.o symbol.o hash_table.o scope.o
	gcc parser.tab.o token.o expr.o decl.o stmt.o type.o param_list.o symbol.o lex.yy.o main.o hash_table.o scope.o -o cminor -Wall -g
main.o: main.c parser.tab.h token.h decl.h parser.tab.h expr.h type.h param_list.h stmt.h symbol.h scope.h
	gcc -c main.c -Wall -g
scope.o: scope.c scope.h hash_table.h symbol.h
	gcc -c scope.c -Wall -g
hash_table.o: hash_table.h hash_table.c
	gcc -c hash_table.c -Wall -g
token.o: token.c token.h parser.tab.h
	gcc -c token.c -Wall -g
param_list.o: param_list.c param_list.h type.h scope.h
	gcc -c param_list.c -Wall -g
type.o: type.c type.h param_list.h expr.h symbol.h
	gcc -c type.c -Wall -g
stmt.o: stmt.c stmt.h decl.h expr.h type.h param_list.h scope.h
	gcc -c stmt.c -Wall -g
decl.o: decl.c decl.h symbol.h scope.h stmt.h expr.h type.h
	gcc -c decl.c -Wall -g
expr.o: expr.c expr.h symbol.h scope.h
	gcc -c expr.c -Wall -g
symbol.o: symbol.c symbol.h type.h param_list.h
	gcc -c symbol.c -Wall -g
parser.tab.o: parser.tab.c parser.tab.h decl.h expr.h stmt.h param_list.h type.h param_list.h
	gcc -c parser.tab.c -Wall -g
lex.yy.o: lex.yy.c parser.tab.h token.h
	gcc -c lex.yy.c -Wall -g
parser.tab.c parser.tab.h: parser.y
	yacc -d -bparser -v parser.y
lex.yy.c: scanner.l
	flex scanner.l
clean:
	rm *.o lex.yy.c parser.tab.c parser.tab.h parser.output cminor
