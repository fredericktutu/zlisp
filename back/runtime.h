/*
 * Item Part 
 * defined in listexec.c
 * 
 * 
 */
typedef enum {
	ITEMTYPE_CONST = 1,
	ITEMTYPE_ID,
	ITEMTYPE_LIST,
	ITEMTYPE_NIL,
	ITEMTYPE_BAD,
	ITEMTYPE_BOOL,
}ItemType;

typedef struct ListContainer ListContainer;

typedef struct Item_{
	ItemType type;
	union{
		int value_const;
		char *value_id;
		ListContainer *value_list;
		int value_bool;
		char *value_bad;
	} value;
}Item;

struct ListContainer {
	Item *head; //the type of head could be all the types defined above
	Item *next; //the type of next should be ITEMTYPE_LIST OR ITEMTYPE_NIL
};


Item *make_empty_item();

Item *make_bad_item(char *c);
Item *make_bool_item(int x);
ListContainer *make_empty_list_container();
ListContainer *make_list_container(Item *first, Item *other);

int free_single_item(Item *it);
int free_list_container(ListContainer *l);

int free_string(char* s);

Item *make_nil_item();

Item *make_id_item(char *s);

Item *make_const_item(int num);

Item *raw_cons(Item *first, Item *other);

Item *raw_car(Item *it);
Item *raw_cdr(Item *it);
Item *raw_is_atom(Item *it);
Item *raw_is_null(Item *it);
Item *raw_is_eq(Item *a, Item *b);

Item *raw_add(Item* x1, Item *x2);
int print_item(Item *it, int n, int in);


/*
 * 
 * Stack Part 
 * 
 * defined in "stack.c"
 * 
 */

typedef struct Stack{
	Item **stack;
	int top;
	int cap;  //capacity
} Stack;

#define STACK_INIT_CAP 100
#define STACK_MAX_CAP 1600

Stack *make_new_stack();
int free_stack(Stack *s);
int stack_push(Stack *s, Item *it);
Item *stack_pop(Stack *s);
Item* get_nth_from_stack(Stack *s, int n);


/*
 * Hash Part
 * will be defined in "hash.c"
 * 
 * 
 */
typedef struct HashObj_{
	struct HashObj_ *next;
	char *key;
	Item *item; 
} HashObj;

typedef HashObj* HashObjPtr;
typedef struct HashTab_{
	HashObjPtr *base; // actually an array
	int length; //the length of the table
	int num;   //the actually number the Hash Table includes, if too big, rehash
	struct HashTab_ *parent; //if null, the Ancestor
} HashTab;

typedef struct ENV{
	HashTab *global;
	HashTab *current;
}ENV;

#define INIT_HASH_LENGTH 253
#define MAX_HASH_RATE 0.7
#define MAX_HASH_LENGTH 253 * 16


//begin TODO
int *init_env();  //use env as global
int *finalize_env(); //free from the current hashtab to the global hashtab

HashTab *init_hashtab(HashTab* parent);
int free_hashtab(HashTab *tab);// free all: (1)hashobj (2)the pointer array of the hashobj (3)the hashtable itself

int initial_scope();
int finalize_scope();

unsigned hash(char *key);
int rehash(HashTab *tab);
Item *lookup_hashtab(HashTab *tab, char* key); //find in the tab, if not find then return NULL
int *add_hashtab(HashTab *tab, char *key, Item *it);

Item *lookup_env(); //find from the current to the global, if not then return NULL
int *add_env(char *key, Item *it); //add to the current
//end TODO
