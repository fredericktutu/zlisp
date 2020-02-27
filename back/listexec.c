 #include <stdio.h>
#include <stdlib.h>
#include "runtime.h"
//TEST PASS


Item *make_empty_item(){
	Item *tmp = (Item*)malloc(sizeof(Item));
	return tmp;
}

Item *make_bad_item(char *c) {
	Item *tmp = make_empty_item();
	tmp -> type = ITEMTYPE_BAD;
	tmp -> value.value_bad = c;
	return tmp;
}

int bad(Item *it) {
	return (it -> type == ITEMTYPE_BAD) ? 1 : 0;
}

Item *make_bool_item(int x) {
	Item *tmp = make_empty_item();
	tmp -> type = ITEMTYPE_BOOL;
	if(x != 0)
		tmp -> value.value_bool = 1;
	else
		tmp -> value.value_bool = 0;
	return tmp;
}
Item *make_nil_item(){
	Item *it = make_empty_item();
	it -> type = ITEMTYPE_NIL;
	return it;
	
}


Item *make_id_item(char *s){
	Item *it = make_empty_item();
	it -> type = ITEMTYPE_ID;
	it -> value.value_id = s;
	return it;
}

Item *make_const_item(int num){
	Item *it = make_empty_item();
	it -> type = ITEMTYPE_CONST;
	it -> value.value_const = num;
	return it; 
}

Item *make_innate_item(ModeType innate_mode, int total_stage) {
	InnateFunction *infunc = malloc(sizeof(InnateFunction));
	infunc -> mode = innate_mode;
	infunc -> total_stage = total_stage;
	Item *it = make_empty_item();
	it -> type = ITEMTYPE_INNATE;
	it -> value.value_innate = infunc;
	return it;
}

ListContainer *make_empty_list_container() {
	ListContainer *tmp = malloc(sizeof(ListContainer));
	return tmp;	
}

ListContainer *make_list_container(Item *first, Item *other) { // after you make sure [other] is a list
	ListContainer *tmp = make_empty_list_container();
	tmp -> head = first;
	tmp -> next = other;
}

int free_single_item(Item *it){
	switch(it -> type){
		case ITEMTYPE_LIST:
			free_list_container(it -> value.value_list);
			break;
		case ITEMTYPE_ID:
			free_string(it->value.value_bad);
			break;
		case ITEMTYPE_BAD:
			free_string(it->value.value_bad);
			break;
		default:
			break;
	}
	free(it);
	return 0;
}
int free_list_container(ListContainer *l){
	free(l);
	return 0;
}

int free_string(char* s){
	free(s);
	return 0;
}





Item *raw_cons(Item *first, Item *other) {
	Item *tmp;
	if(other -> type == ITEMTYPE_LIST || other -> type == ITEMTYPE_NIL) {
		tmp = make_empty_item();
		tmp -> type = ITEMTYPE_LIST;
		tmp -> value.value_list = make_list_container(first, other);
		return tmp;
	} else {
		char *c = "the second should be a list";
		return make_bad_item(c);  // if the second is not a list, return a bad item
	}
}

Item *raw_car(Item *it) {
	Item *tmp;
	if(it -> type != ITEMTYPE_LIST){
		char *c = "not a list";
		return make_bad_item(c);
	} else {
		return it -> value.value_list -> head; 
	}
}

Item *raw_cdr(Item *it) {
	Item *tmp;
	if(it -> type != ITEMTYPE_LIST){
		char *c = "not a list";
		return make_bad_item(c);
	} else {
		return it -> value.value_list -> next; 
	}
}


Item *raw_is_atom(Item *it){
	if(it -> type == ITEMTYPE_CONST || it -> type == ITEMTYPE_ID)
		return make_bool_item(1);
	else
		return make_bool_item(0);
}

Item *raw_is_null(Item *it){
	int x = (it -> type == ITEMTYPE_NIL)? 1 : 0;
	return make_bool_item(x);	
}

Item *raw_is_eq(Item *a, Item *b){
/*
 * the eq? function test whether a and b are the [same thing]
 * One situation, if a and b are the same ID or CONST or NIL or bool, then they are eq
 * Another situation, if a and b are both function(lambda/macro) or list(nature/stream), but they are actually the [same thing](the same eternal Item id).
 *******************************
 >(define x (cons 1 nil)) 
 >(define y x)
 >(eq? x (cons 1 nil)) 
 #f
 >(eq? x y) 
 #t
 ****************************
 * at this level, we only deal with Item-level, we temperarily ignore the symbol table, and ignore the eval-stage of the top 2 things in the stack
 * so now given two Items, we check whether they are eq
 */
	if(a == b) return make_bool_item(1); //if the same eternally, they must be eq
	if(a -> type == b -> type){
		if(a -> type == ITEMTYPE_NIL){
			return make_bool_item(1);
		} else if(a -> type == ITEMTYPE_BOOL && a -> value.value_bool == b -> value.value_bool) {
			return make_bool_item(1);
		} else if(a -> type == ITEMTYPE_CONST && a -> value.value_const == b -> value.value_const) {
			return make_bool_item(1);
		} else if(a -> type == ITEMTYPE_ID && a -> value.value_id == b ->value.value_id) {
			return make_bool_item(1);
		} else {
			return make_bool_item(0);
		}
	//end big if	
	} else {
		return make_bool_item(0);
	}
}

Item *raw_add(Item* x1, Item *x2){
	Item *res = make_const_item(0);
	res -> value.value_const = (x1 -> value.value_const + x2 -> value.value_const);
	return res;
}

int print_blank(int n){
	for(int i=0;i<n;i++){
		printf(" ");
	}
}
int print_item(Item *it, int n, int in){
	switch(it -> type){
		case ITEMTYPE_BOOL:
			printf("(BOOL):");
			if(it -> value.value_bool == 1){
				printf("#t");
			} else {
				printf("#f");
			}
			break;
		case ITEMTYPE_CONST:
			printf("(Constant): %d", it -> value.value_const);
			break;
		case ITEMTYPE_ID:
			printf("(ID): %d", it -> value.value_const);
			break;
		case ITEMTYPE_LIST:
			printf("(List): ");
			printf("[");
			print_item(it -> value.value_list -> head, 0, 1);
			printf("] ->\n");
			print_blank(n + 2);
			print_item(it -> value.value_list -> next, n + 2, 1);
			break;
		case ITEMTYPE_NIL:
			printf("(Nil)");
			break;
		case ITEMTYPE_BAD:
			printf("(Bad): %s", it -> value.value_bad);
			break;
		default:
			printf("error!");
	}
	if(in == 0){
		printf("\n");
	}	
}

