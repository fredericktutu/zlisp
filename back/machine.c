#include "runtime.h"
#include "machine.h"
#include <string.h>
//mode(stage) stack, can see the current mode[stage]
//by the current mode[stage], we give out a evaluate method

Stack *ModeStack; //to store all the modes 
Stack *WorkStack; //to store all the items
Method current_method;

char* innate_function_names[] = {"cons", "car", "cdr", "define"}; 
ModeType innate_function_modes[] = {MODE_CONS,MODE_CAR,MODE_CDR,MODE_DEFINE};
int innnate_function_totalstages[] = {4, 3, 3, 4};
Item* make_mode_item(ModeType type, int total_stage) {
    Mode* mode = malloc(sizeof(Mode));
    mode -> type = type;
    mode -> total_stage = total_stage;
    mode -> stage = 0;

    Item* tmp = malloc(sizeof(Item));
    tmp -> type = ITEMTYPE_MODE;
    tmp -> value.value_mode = mode;

    return tmp;
}

Mode* current_mode() {
    Item *mode_it = get_nth_from_stack(ModeStack, 0);
    return mode_it ->value.value_mode;
}

int current_stage() {
    return current_mode() -> stage;
}
int current_total_stage() {
    return current_mode() -> total_stage;
}

int new_mode(ModeType type, int total_stage) {
    stack_push(ModeStack,make_mode_item(type, total_stage));
    return 0;
}

//for pushi


int end_mode() {
    free(stack_pop(ModeStack));
    return 0;
}

int is_start_mode() {
    return (MODE_START == current_mode() -> type) ? 1 : 0;
}


int incr_stage() {
    current_mode() -> stage ++;
    return 0;
}

//n start from 1
Item* get_nth_var(int total, int n) {
	return get_nth_from_stack(WorkStack, n - total);
}

int pop_num(int num) {
    for(int i=0;i<num;i++) {
        stack_pop(WorkStack);
    }
}

int add_innate_functions() {
    for(int i=0; i<sizeof(innate_function_modes)/sizeof(innate_function_modes[0]); i++) {
        add_env(innate_function_names[i], make_innate_item(innate_function_modes[i], innnate_function_totalstages[i]));
    }
}

int init_machine() {
    ModeStack = make_new_stack();
    WorkStack = make_new_stack();
    stack_push(ModeStack, make_mode_item(MODE_START, 0));

    init_env();
}

int shutdown_machine() {
    free_stack(ModeStack);
    free_stack(WorkStack);
    finalize_env();
}




int run_instr(Instruction * instr) {
    Item *it;
    switch(current_method) {
        METHOD_EVAL: //push id or constant, and eval it
        {
            if(instr -> type == INSTR_PUSHC) {
                it = make_const_item(instr -> ex1.constant);
                stack_push(WorkStack, it);
            }else if(instr -> type == INSTR_PUSHI) {
                Item * func = lookup_env(instr -> ex1.id);
                if(func == NULL || 
                        !(func -> type == ITEMTYPE_INNATE || func -> type == ITEMTYPE_LAMBDA)) {
                    //error: undefined function
                } else {
                    stack_push(WorkStack, func);
                }
            } else {
                //reserved
            }
            break;
        }
        METHOD_NOT_EVAL: //only push id, and not eval this
        {
            if(instr -> type == INSTR_PUSHI) {
                stack_push(WorkStack, make_const_item(instr -> ex1.constant));
            } else if(instr -> type == INSTR_PUSHC) {
                stack_push(WorkStack, make_id_item(instr ->ex1.id));
            } else {
                //reserved
            }
            break;
        }
        METHOD_PASS: //do nothing
        {
            break;
        }
        METHOD_APP: 
        {
            switch(current_mode() ->type) {
                case MODE_CONS:
                {
                    it = raw_cons(get_nth_var(2, 1), get_nth_var(2,2));
                    pop_num(3);
                    stack_push(WorkStack, it);
                }
                case MODE_CAR:
                {
                    it = raw_car(get_nth_var(1,1));
                    pop_num(2);
                    stack_push(WorkStack, it);
                }
                case MODE_CDR:
                {
                    it = raw_cdr(get_nth_var(1,1));
                    pop_num(2);
                    stack_push(WorkStack, it);
                }
                case MODE_DEFINE:
                {
                    add_env(get_nth_var(2, 1) -> value.value_id, get_nth_var(2, 2));
                    
                    it = get_nth_var(2, 1);
                    pop_num(3); 
                    stack_push(WorkStack, it);
                }
            }
        }
    }
}

int run_all() {
    while(has_instr()) {
        Instruction* instr = get_next_instr();
        change_mode(instr);
        run_instr(instr);
    }

    free_stack(ModeStack);
    free_stack(WorkStack);

}

//It should change the current mode, according to current instr, and then set the evaluation method

int change_mode(Instruction *instr) {
    Mode *cur = current_mode();
    Item *it;
    //the current code is to 
    int flag = 0;
    incr_stage();
    int s = current_stage();
    switch(cur -> type) {
        case MODE_SINGLE:
        {
            if(s == 1)flag = 1;
        }
        case MODE_CONS:
        case MODE_CAR:
        case MODE_CDR:
        {
            if(s == current_total_stage())flag = 1;
        }
    }

    if(flag == 1) {
        end_mode();
    }

    switch(cur -> type) {
        //begin(mode start)
        case MODE_START: 
        {   
            switch(instr -> type) {
                case INSTR_OP:
                {
                    new_mode(MODE_OP, 3);
                    current_method = METHOD_PASS;
                    break;
                }
                case INSTR_PUSHI:
                case INSTR_PUSHC:
                {
                    new_mode(MODE_SINGLE, 2);
                    current_method = METHOD_EVAL;
                    break;
                }
                case INSTR_POP:
                {
                    //just pop, and maybe there isn't things to pop, but this kind must be a wrong code
                    break;
                }
                default:
                {
                    //error
                }
            }
            break;
        }
        //end(mode start)
        //begin(mode op)
        case MODE_OP:
        {
            if(current_stage() == 1) {
                switch(instr -> type) {
                    case INSTR_OP:
                    {
                        new_mode(MODE_OP, 2);
                        current_method = METHOD_PASS;
                        break;
                    }
                    case INSTR_PUSHI:
                    {
                        current_method = METHOD_EVAL;
                        break;
                    }
                    default:
                    {
                        //error
                    }

                }
            } else if(current_stage() == 2) {
                if(instr -> type == INSTR_CHECK) {
                    //TODO: transit to the next mode, and set the method to be pass, and wait for the next move
                    it = get_nth_from_stack(WorkStack, 0);
                    if(it -> type == ITEMTYPE_INNATE) {
                        new_mode(it -> value.value_innate->mode,it -> value.value_innate ->total_stage);
                        current_method = METHOD_PASS;
                    } else if(it -> type == ITEMTYPE_LAMBDA) {
                        //reserved
                    }
                    current_method = METHOD_PASS;
                } else {
                    //error
                }
            }
        }
        //end(mode op)
        //begin(mode cons,car,cdr)
        case MODE_CONS:
        case MODE_CAR:
        case MODE_CDR:
        {
            //this kind, all the arguments are evaluated, and they are innate function
            if(current_stage() == current_total_stage() - 1) {
                if(instr -> type = INSTR_DOL) {
                    //start to eval the innate function, set method(APP)
                    current_method = METHOD_APP;
                } else {
                    //error: too many arguments
                }
            } else { // an arugument

                switch(instr -> type) {
                    case INSTR_PUSHC:
                    {
                        current_method = METHOD_EVAL;
                        break;
                    }
                    case INSTR_PUSHI:
                    {
                        current_method = METHOD_EVAL;
                        break;
                    }
                    case INSTR_OP:
                    {
                        new_mode(MODE_OP,3);
                        current_method = METHOD_PASS;
                        break;
                    }
                    default:
                    {
                        //error: there should be an arugument
                    }
                }

            }
        }
        //end(mode cons,car,cdr)
        //begin(mode define)
        case MODE_DEFINE:
        {
            switch(current_stage()) {
                case 1:
                {
                    if(instr -> type == INSTR_PUSHI) {
                        current_method = METHOD_NOT_EVAL;
                    } else {
                        //error
                    }
                    break;
                }
                case 2:
                {
                    switch(instr -> type) {
                        case INSTR_PUSHI:
                        case INSTR_PUSHC:
                        {
                            current_method = METHOD_EVAL;
                        }
                        case INSTR_OP:
                        {
                            new_mode(MODE_OP, 3);
                            current_method = METHOD_PASS;
                        }
                        default:
                        {
                            //error
                        }
                    }
                    break;
                }
                case 3:
                {
                    if(instr -> type = INSTR_DOL) {
                        current_method = METHOD_APP;
                    } else {
                        //error: too many arguments
                    }
                }
            }
        }
        //end(mode define)
    }
}