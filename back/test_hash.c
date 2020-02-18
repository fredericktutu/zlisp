#include "runtime.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
    Item *tmp;
    char *n1 = "x1";
    char *n2 = "x2";
    printf("---test suite1---\n");
    printf("init environment:\n");
    init_env();
    
    printf("bind x1 = 1, x2 = 1\n");
    tmp = make_const_item(1);
    add_env(n1, tmp);
    add_env(n2, tmp);

    printf("lookup x1, expect 1\n");
    print_item(lookup_env(n1), 0, 0);
    printf("lookup x2, expect 1\n");
    print_item(lookup_env(n2), 0, 0);


    printf("---test suite2(new scope)---\n");
    initial_scope();
    printf("bind x1 = 2\n");
    tmp = make_const_item(2);
    add_env(n1, tmp);

    printf("lookup x1, expect 2\n");
    print_item(lookup_env(n1), 0, 0);
    printf("lookup x2, expect 1\n");
    print_item(lookup_env(n2), 0, 0);

    printf("---test suite3(finalize the scope)---\n");
    finalize_scope();
    
    printf("lookup x1, expect 1\n");
    print_item(lookup_env(n1), 0, 0);
    printf("lookup x2, expect 1\n");
    print_item(lookup_env(n2), 0, 0);

    finalize_env();

    printf("all test passed\n");
}