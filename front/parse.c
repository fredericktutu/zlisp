#include "sexp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
Atom *make_atom(){
	return (Atom *)malloc(sizeof(Atom));
}

Atom *make_num(int val){
	Atom *ato = make_atom();
	ato -> type = TYPE_NUM;
	ato -> number_value = val;
	return ato;
}

Atom *make_id(char *val){
	Atom *ato = make_atom();
	ato -> type = TYPE_ID;
	ato -> string_value = val;
	return ato;
}

SExpr *make_nil(){
	SExpr *S = malloc(sizeof(SExpr));
	S -> head_type = TYPE_NIL;
	S -> list_start = 0;
	return S;
}

SExpr *make_empty_s(){
	SExpr *s = malloc(sizeof(SExpr));
	s -> next = NULL;
	s -> list_start = 0;
	return s;
}

int *cons_s(SExpr *head, SExpr *other){
	head -> next = other;
	return 0;
}



int print_sexpr(SExpr *s){
	if(s -> list_start == 1){
		printf("(");
		SExpr *tmp = s;
		print_current(tmp);
		tmp = tmp -> next;
		while(tmp != NULL){
			printf(" ");
			print_current(tmp);
			tmp = tmp -> next;
		}
		printf(")");
	}else{		
		print_current(s);
	}
	
}

int print_current(SExpr *s){
	Atom *a;
	switch(s -> head_type){
		case TYPE_ATOM:
			a = (s -> head).atom;
			if(a->type == TYPE_NUM){
				printf("const");
			}else{
				printf("id");
			}
			break;
		case TYPE_NIL:
			printf("()");
			break;
		case TYPE_LIST:
			print_sexpr(s -> head.list);
			break;
	}
	
}


