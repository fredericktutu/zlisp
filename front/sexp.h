typedef enum {
	TYPE_NUM = 1,
	TYPE_ID
}AtomType;

typedef enum{
	TYPE_ATOM = 1,
	TYPE_LIST,
	TYPE_NIL
}SType;


typedef struct Atom{
	AtomType type;
	int number_value;
	char *string_value;
}Atom;

typedef struct SExpr{
	int list_start;
	SType head_type;
	union {
		Atom *atom;
		struct SExpr *list;
	}head;
	struct SExpr *next;
}SExpr;

Atom *make_atom();
Atom *make_num(int val);
Atom *make_id(char *val);
SExpr *make_nil();
SExpr *make_empty_s();
int *cons_s(SExpr *head, SExpr *other);
int print_current(SExpr *s);
int print_sexpr(SExpr *s);
