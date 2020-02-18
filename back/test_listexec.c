#include "runtime.h"
#include <stdlib.h>
#include <stdio.h>

int main(){
	Item* x1 = make_const_item(10);
	Item* x2 = make_const_item(10);
	Item* ni = make_nil_item();
	Item* x3 = raw_add(x1, x2);
	Item* b1 = raw_is_atom(ni);
	Item* b2 = raw_is_eq(x1, x3);
	Item* l1 = raw_cons(x1, raw_cons(x3, raw_cons(ni, raw_cons(b1, raw_cons(b2, ni)))));
	print_item(l1, 0, 0);
	Item* list[] = {x1, x2, ni, x3, b1, b2, l1};
	for(int i = 0; i< 7; i++){
		free_single_item(list[i]);
	}
	return 0;
}
