CC=gcc
CFLAGS=-c -Wall -g
HEADERS=decl.h expr.h hash_table.h param_list.h parser.tab.h register.h scope.h stmt.h symbol.h token.h type.h
OBJECTS=$(HEADERS:.h=.o) lex.yy.o main.o

#cminor is the entire target
all: cminor
#Build the executable using all object files
cminor: $(OBJECTS)
	gcc $^ -o $@ -Wall -g
#Build all .o files using matching .c files
%.o: %.c
	$(CC) $(CFLAGS) $<
#Run bison on appropriate file(s)
%.tab.c %.tab.h: %.y
	yacc -d -bparser -v $<
#Build lex file
lex.yy.c: scanner.l
	flex scanner.l
#For simplicity, remake all object files anytime a header file is changed
$(OBJECTS): $(HEADERS)
#Clean: remove all intermediate files
clean:
	\rm *.o lex.yy.c parser.tab.c parser.tab.h parser.output cminor
