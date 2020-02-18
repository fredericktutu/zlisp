#include "runtime.h"
#include <stdlib.h>
#include <stdio.h>
//TEST PASS


Stack *make_new_stack() {
	Stack *s = malloc(sizeof(Stack));
	s -> stack = malloc(sizeof(Item*) * STACK_INIT_CAP);
	s -> top = -1;
	s -> cap = STACK_INIT_CAP;
}

int free_stack(Stack *s) { 
	free(s -> stack);
	free(s);
	return 0;
}

int realloc_stack(Stack *s) {
	if(s -> cap >= STACK_MAX_CAP) {
		printf("stack explodes\n");
		exit(-1);
	}
	Item** new_stack = malloc(sizeof(Item*) * (s -> cap) * 2);
	for(int i=0; i< s -> cap; i++){
		new_stack[i] = s -> stack[i];
	}
	s -> cap = s -> cap * 2;
	free(s -> stack);
	s -> stack = new_stack;
	return 0;
}

int stack_push(Stack *s, Item *it) {
	if(s -> top == s -> cap -1) {
		realloc_stack(s);
	}
	s -> top ++;
	s -> stack[s -> top] = it;
	return 0;
}

Item *stack_pop(Stack *s) {
	if(s->top == -1){
		return NULL;
	} else {
		Item *it = s -> stack[s -> top];
		s -> top = s -> top - 1;
		return it;
	}
}


Item* get_nth_from_stack(Stack *s, int n){
	if(s -> top < n) {
		return NULL;
	} else {
		return s -> stack[s -> top - n];
	}
}










