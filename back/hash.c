#include "runtime.h"
#include <stdlib.h>
#include <string.h>
//TESTING

ENV *environment;



int *init_env() {  //use env as global
	environment = malloc(sizeof(ENV));
	environment -> current = environment -> global = init_hashtab(NULL); //the global scope
	return 0;
}
int *finalize_env() { //free from the current hashtab to the global hashtab
	HashTab *cur = environment -> current;
	HashTab *tmp;
	while(cur != NULL){
		tmp = cur -> parent;
		free_hashtab(cur);
		cur = tmp;
	}
	return 0;
}
HashTab *init_hashtab(HashTab* parent) {
	HashTab *new_tab = malloc(sizeof(HashTab));
	new_tab -> num = 0;
	new_tab -> length = INIT_HASH_LENGTH;
	new_tab -> parent = parent;
	new_tab -> base = malloc(sizeof(HashObjPtr) * INIT_HASH_LENGTH);
	HashObjPtr *base = new_tab -> base; 
	for(int i=0;i<INIT_HASH_LENGTH;i++) {
		base[i] = NULL;
	}
	return new_tab;
}
int free_hashtab(HashTab *tab) {// free all: (1)hashobj (2)the pointer array of the hashobj (3)the hashtable itself
	for(int i=0;i<tab->length;i++) {
		free(tab -> base[i]);
	}
	free(tab -> base);
	free(tab);
	return 0;
}


int initial_scope() {
	if(environment == NULL) return 1;
	HashTab *new_tab = init_hashtab(environment -> current);
	new_tab -> parent = environment -> current;
	environment -> current = new_tab;
	return 0;
}
int finalize_scope() {
	if(environment == NULL || environment -> current == NULL) return 1;
	HashTab *cur_tab = environment -> current;
	environment -> current = cur_tab -> parent;
	return 0;
}

unsigned hashfunc(int length, char *key) {
	unsigned hashval;
	for (hashval = 0;*key != '\0'; key++) {
		hashval = *key + 31 * hashval;
	}
	//printf("hashval: %u\n",hashval);
	return hashval % length;
}

int rehash(HashTab *tab) {
	if(tab -> length >= MAX_HASH_LENGTH) exit(-1);
	
	int new_length = tab -> length * 2;
	HashObjPtr *new_base = malloc(sizeof(HashObjPtr) * new_length);
	
	int i;
	for(i=0; i < tab -> length; i++) {
		new_base[i] = tab -> base[i];
	}
	for(i = tab -> length; i < new_length; i++){
		new_base = NULL;
	}
	free(tab -> base);
	tab -> base = new_base;
	tab -> length = new_length;
	return 0;
	
}


HashObjPtr make_hashobj(char *key, Item *it){
	HashObjPtr opt = malloc(sizeof(HashObj));
	opt -> next = NULL;
	opt -> key = malloc(sizeof(key));
	strcpy(opt -> key, key);
	opt -> item = it;
	return opt;
}

Item *lookup_hashtab(HashTab *tab, char* key) {
 //find in the tab, if not find then return NULL
	unsigned hashval = hashfunc(tab -> length, key);
	if(tab -> base[hashval] == NULL) {return NULL;}
	HashObjPtr opt = tab -> base[hashval];
	while(opt != NULL && strcmp(opt -> key ,key)) {
		opt = opt -> next;
	}
	if(opt == NULL) {
		return NULL;
	}
	else {
		return opt -> item;
	}
}



int *add_hashtab(HashTab *tab, char *key, Item *it) {
	if(tab -> num / tab -> length >= MAX_HASH_RATE && rehash(tab)) return 1;
	unsigned hashval = hashfunc(tab -> length, key);
	HashObjPtr *base = tab -> base;
	if(base[hashval] == NULL) {
		base[hashval] = make_hashobj(key, it);
	} else {
		HashObjPtr opt = base[hashval];
		while(opt -> next != NULL){
			opt = opt -> next;
		}//then the base is the last base
		opt -> next = make_hashobj(key, it);
	}
	return 0;
}

Item *lookup_env(char *key) {
	//find from the current to the global, if not then return NULL
	if(environment == NULL) return NULL;
	HashTab *cur = environment -> current;
	Item *it;
	while(cur != NULL && ((it = lookup_hashtab(cur, key)) == NULL)){ //if not find, find in the parent
		cur = cur -> parent;
	}
	
	return it;
}

int *add_env(char *key, Item *it) {
	//add to the current
	if(add_hashtab(environment -> current, key, it)) return 1;
	else return 0;
}
