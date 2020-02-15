%{
#include <stdio.h>
#include "y.tab.h"
#include "sexp.h"
#include <string.h>
#define YYSTYPE SExpr*
extern SExpr * yylval;

%}

%%
"("  return LP;
")"  return RP;
"'"  return QUO;

[0-9]+   {
	int tmp;
	sscanf(yytext, "%d", &tmp);
	Atom *atom_num = make_num(tmp);
	SExpr *s = make_empty_s();
	s -> head_type = TYPE_ATOM;
	s -> head.atom = atom_num;
	yylval = s;
	return NUMBER;
}

[A-Za-z_][A-Za-z_0-9?]*    {
	char *id = (char *) malloc(sizeof(char) * yyleng + 1);
    strcpy(id, yytext);
	Atom *atom_id = make_id(id);
	SExpr *s = make_empty_s();
	s -> head_type = TYPE_ATOM;
	s -> head.atom = atom_id;
	yylval = s;
	return ID;
}

[ \t\n]+      { ; }

.   {
	fprintf(stderr, "lexcial error.\n"); 
	exit(1);
}
%%
int
yywrap(void){
	return 1;
}
