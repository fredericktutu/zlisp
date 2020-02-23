/*
pushs
pushf

dol x

pop
*/



int write_once(char* code, char *path);
char* codegen(SExpr *exp);
char* gen_exp(SExpr *exp);
char* gen_current(SExpr *exp);