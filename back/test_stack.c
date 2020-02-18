#include "runtime.h"
#include <stdio.h>



int main(){
    printf("start test make\n");
    Stack *astack = make_new_stack();
    Item *tmp;
    printf("start test push\n");
    for(int i=0;i<5;i++) {
        tmp = make_const_item(i);
        printf("yield");
        print_item(tmp, 0, 0);
        stack_push(astack, tmp);
    }
    printf("start test pop\n");
    for(int i=0;i<5;i++) {
        tmp = stack_pop(astack);
        printf("get");
        print_item(tmp, 0, 0);
    }
    



    free_stack(astack);
	
	
	
}
