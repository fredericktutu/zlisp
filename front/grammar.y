%{
#include <stdio.h>
#include <stdlib.h>
#include "sexp.h"

#define YYDEBUG 1
#define YYSTYPE SExpr*
%}

%token LP RP QUO
%token NUMBER ID
%%
program:
    expression
    {
		print_sexpr($1);
    }
    ;
expression:
	NUMBER
	|ID
	|LP RP
	{
		$$ = make_nil();
	}
	|LP expression_list RP
	{
		$2 -> list_start = 1;
		$$ = $2;
	}
	|QUO expression
	{
		if($2 -> list_start == 1){
			SExpr *tmp = make_empty_s();
			tmp -> head_type = TYPE_LIST;
			tmp -> head.list = $2;
			$2 = tmp;
		}
		char* s = "quote";
		Atom *a = make_id(s);
		SExpr *atom_s = make_empty_s();
		atom_s -> head_type = TYPE_ATOM;
		atom_s -> head.atom = a;
		atom_s -> list_start = 1;
		cons_s(atom_s, $2);
		$$ = atom_s;
	}
	;

expression_list:
	expression
	{
		if($1 -> list_start == 1){
			SExpr *res;
			res = make_empty_s();
			res -> head_type = TYPE_LIST;
			res -> head.list = $1;
			$$ = res;
		}else{
			$$ =$1;
		}
	}
	|expression expression_list
	{	
		if($1 -> list_start == 1){
			SExpr *res;
			res = make_empty_s();
			res -> head_type = TYPE_LIST;
			res -> head.list = $1;
			cons_s(res, $2);
			$$ = res;
		}else{
			cons_s($1, $2);
			$$ = $1;
		}
	}
	;

%%
int yyerror(char const *str){
    extern char *yytext;
    fprintf(stderr, "parser error near %s\n", yytext);
    return 0;
}

int main(void){
    extern int yyparse(void);
    extern FILE *yyin;
    yyin = stdin;
    if (yyparse()){
        fprintf(stderr, "Error!\n");
        exit(1);
    }
}
