CC = gcc

all: lisp
COMOBJ = y.tab.o lex.yy.o parse.o codegen.o

#----------------------------------------
.c.o:
	 $(CC)  -pg  -Wall $(INCLUDE) -c $(CFLAGS) $(MODEL) $*.c 
	 
#----------------------------------------


y.tab.c y.tab.h: grammar.y
	byacc -dv grammar.y

lex.yy.c: lexer.l
	flex lexer.l
	
#------------------------------------------
lisp:  $(COMOBJ) 
	$(CC) -pg  -o  lisp $(LIB) $(MODEL) $(COMOBJ) $(CLIB)
#------------------------------------------
clean: 
	rm *.o y.tab.c y.tab.h lex.yy.c
	
#
lex.yy.o: lex.yy.c sexp.h y.tab.h
parse.o:   sexp.h parse.c
y.tab.o:  y.tab.c sexp.h codegen.h
codegen.o: codegen.c codegen.h

