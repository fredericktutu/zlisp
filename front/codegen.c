#include "sexp.h"
#include "codegen.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* PUSHI = "pushi";
char* PUSHC = "pushc";
char* PUSHN = "pushn";
char* CHECK = "check";
char* DOL   = "dol";
char* POP   = "pop";
char* SP    = " ";
char* NIL   = "nil";
char* FEED    = "\n";

/*
int main() {
    write_once("hello world\n", "testcase1.zv");
    return 0;
}
*/

int write_once(char* code, char *path) {
    FILE *fpWrite = fopen(path, "a");
    if(fpWrite == NULL)return 0;

    fprintf(fpWrite, "%s", code);
    fclose(fpWrite);
}






char* codegen(SExpr *exp) {
    char* res_of_exp = gen_exp(exp);
    char* res = malloc(strlen(res_of_exp) + strlen(POP) + strlen(FEED) + 1);
    sprintf(res, "%s%s%s", res_of_exp, POP, FEED);
    free(res_of_exp);
    return res;
}

int len(int x) {
    int count = 1;
    while(x >= 10) {
        x /= 10;
        count++;
    }
    return count;
}

char* gen_exp(SExpr *exp){
    char* res;
    char* dump;
    char* res_of_sub;
    if(exp -> list_start == 1) { //list
        int count = 1;
        char* string_of_count;
        //printf("at head:\n");
        res_of_sub = gen_current(exp);
        res = malloc(strlen(res_of_sub) + strlen(CHECK) + strlen(FEED) + 1);
        sprintf(res, "%s%s%s", res_of_sub, CHECK, FEED);
        //printf("at sprintf\n");
        free(res_of_sub);
        //printf("finish head\n");
        SExpr *tmp = exp -> next;
        while(tmp != NULL) {
            count ++;
            res_of_sub = gen_current(tmp);
            
            dump = res;
            res = malloc(strlen(dump) + strlen(res_of_sub) + 1);
            sprintf(res, "%s%s", dump, res_of_sub);
            //printf("before free, the dump is:\n %s\n",dump);
            free(dump);
            //printf("after free\n");
            
            tmp = tmp -> next;
        }

        dump = res;
        string_of_count = malloc(len(count) * sizeof(char) + 1);
        sprintf(string_of_count,"%d", count);
        res = malloc(strlen(dump) + strlen(DOL) + strlen(SP) + strlen(string_of_count) + strlen(FEED) + 1);
        sprintf(res,"%s%s%s%s%s",dump,DOL,SP,string_of_count,FEED);
        //printf("free string of count\n");
        free(string_of_count);
        //printf("free dump, which is  %s\nand res is %s", dump, res);
        free(dump);
        //printf("finish free dump\n");
        return res;
        
    } else {//atom
        res = gen_current(exp);
        return res;
    }
}


char* gen_current(SExpr *exp) {
    char* res;
    Atom *ato;
    switch(exp -> head_type) {
        case TYPE_ATOM:
            ato = exp -> head.atom;
            switch(ato -> type) {
                case TYPE_NUM:
                    {                
                    int count = len(ato -> number_value);
                    char* string_of_number = malloc(count * sizeof(char) + 1);
                    sprintf(string_of_number,"%d", ato -> number_value);
                    res = malloc(strlen(PUSHC) + strlen(SP) + len(string_of_number) * sizeof(char) + strlen(FEED) + 1);
                    sprintf(res, "%s%s%s%s", PUSHC, SP, string_of_number, FEED);
                    free(string_of_number);
                    return res;
                    break;
                    }

                case TYPE_ID:
                    {
                    res = malloc(strlen(PUSHI) + strlen(SP) + strlen(ato -> string_value) + strlen(FEED) + 1);
                    sprintf(res, "%s%s%s%s", PUSHI, SP, ato -> string_value, FEED);
                    return res;
                    }
            }
            break;
        case TYPE_NIL:
            {
            res = malloc(strlen(PUSHN) + strlen(FEED) + 1);
            sprintf(res, "%s", PUSHN);
            return res;
            break;
            }
        case TYPE_LIST:
            {
            res = gen_exp(exp -> head.list);
            return res;
            break;
            }
    }
}