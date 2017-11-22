#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "symtable.h"
#include "token.h"
#include "frame.h"
#include "abstract_tree.h"
#include "generator.h"
#include "error.h"
#include "name_generator.h"
#include "gen_stacks.h"
#include "at_que.h"
#include "string_storage.h"

extern Tframe* temp_frame;
TFstack * frame_stack = NULL;

void generate_start(ATQueue *queue){
    frame_stack = malloc(sizeof(TFstack));
    FS_init(frame_stack);
    create_frame();
    push_frame(frame_stack,NULL,0);
    open_output();
    generate_program(queue);
    pop_frame(frame_stack);
}

void generate_program(ATQueue *queue){
    while(!queEmpty(queue)){
        ATQItem * item = queFront(queue);
        GenValue value = item->GenValue;
        GenType type = item->GenType;
        if(type == gt_var_declar)
            generate_variable_declaration(value.var_declar_input->id,value.var_declar_input->expr);
        else if(type == gt_func_declar)
            generate_function(value.func_declar_input->sym_item,(ATQueue*)value.func_declar_input->queue);
        else if(type == gt_assign)
            generate_assign(value.assign_input->id,value.assign_input->expr);
        else if(type == gt_input)
            generate_input(value.id);
        else if(type == gt_print)
            generate_print(value.exprs);
        else if(type == gt_if)
            generate_if(value.if_input->cond_expr,(ATQueue*) value.if_input->true_queue,(ATQueue*)value.if_input->false_queue);
        else if(type == gt_while)
            generate_while(value.while_input->cond_expr,(ATQueue*) value.while_input->queue);
        else if(type == gt_call_func)
            generate_call_function(value.call_func_input->id,value.call_func_input->sym_item, value.call_func_input->param);
        else if(type == gt_return)
            generate_return(value.return_input->sym_item, value.return_input->expr);
        else if(type == gt_main)
            generate_main((ATQueue*)value.at_queue);
        else{
            printf("COMPILER ERROR\n");
            exit(COMPILER_ERROR);
        }
        queRemove(queue);
    }

}

void generate_main(ATQueue * queue){
    create_frame();
    push_frame(frame_stack,NULL,0);
    fprintf(stdout,"LABEL $$main\nCREATEFRAME\nPUSHFRAME\n");
    generate_program(queue);
    fprintf(stdout,"POPFRAME\n");
    pop_frame(frame_stack);
}

void generate_variable_declaration(Tsymtab_item * id, ATLeaf * expr){
    id = id;
    expr = expr;
    //Marek, po deklaraci vlozi do ramce v c 
}

void generate_assign(Tsymtab_item* id, ATLeaf * expr){
    id = id;
    expr = expr;
    //Marek
}

void generate_input(Tsymtab_item * id){
    id = id;
    //Marek
}

void generate_print(eQueue * exprs){
    exprs = exprs;
    //Marek
}

void generate_call_function(Tsymtab_item * id, Tsymtab_item * sym_item, eQueue * param){
    create_frame();
    fprintf(stdout,"CREATEFRAME\n");
    for(int i = 0; i < sym_item->type_strct.function->arg_count;i++){
        fprintf(stdout,"DEFVAR TF@%s\n",sym_item->type_strct.function->arguments[i].key);
        char * prom = generate_expression(equeFront(param));
        fprintf(stdout,"MOVE TF@%s LF@%s\n",sym_item->type_strct.function->arguments[i].key,prom);
        // Pridat argumenty do TF C
    }
    fprintf(stdout,"CALL $%s\n",sym_item->key);
    fprintf(stdout,"MOVE GF@%s TF@%%retval\n",id->key);
}

void generate_return(Tsymtab_item * sym_item, PrecendentOutput * expr){
    expr = expr;
    sym_item = sym_item;
    //Marek
}

void open_output(){
    fprintf(stdout,".IFJcode17\nJUMP $$main\n");
}

void generate_function(Tsymtab_item * item, ATQueue * state){
    fprintf(stdout, "LABEL $%s\n",item->key);
    push_frame(frame_stack,NULL,0);
    fprintf(stdout, "PUSHFRAME\nDEFVAR LF@%%retval\n");
    Tfunction_item *  function = item->type_strct.function;
    if(function->return_type == type_doub){
        fprintf(stdout,"MOVE LF@%%retval float@0.0\n");
    }
    else if(function->return_type == type_int){
        fprintf(stdout,"MOVE LF@%%retval int@0\n");
    }
    else if(function->return_type == type_str){
        fprintf(stdout,"MOVE LF@%%retval string@\n");
    }
    generate_program(state);
    fprintf(stdout, "LABEL $%s$epilog\n",item->key);
    fprintf(stdout, "POPFRAME\nRETURN\n");
    pop_frame(frame_stack);
}

void generate_if(ATLeaf * condition, ATQueue * state_true, ATQueue * state_false){
    fprintf(stdout,"CREATEFRAME\n");
    create_frame();
    // NAhazet do TF
    push_frame(frame_stack,NULL,0);
    fprintf(stdout,"PUSHFRAME\n");
    char *label = generate_name(gt_label);
    char *cond = generate_expression(condition);
    char * end_label = generate_name(gt_label);
    fprintf(stdout, "JUMPIFNEQ %s bool@true LF@%s\n",label,cond);
    generate_program(state_true);
    fprintf(stdout,"JUMP %s\nLABEL %s\n",end_label,label);
    generate_program(state_false);
    fprintf(stdout,"LABEL %s\n",end_label);
    fprintf(stdout,"POPFRAME\n");
    pop_frame(frame_stack);
    //Vyhazet z TF
}

void generate_while(ATLeaf * condition, ATQueue * state){
    char *label = generate_name(gt_label);
    char * end_label = generate_name(gt_label);
    fprintf(stdout,"LABEL %s\n",label);
    fprintf(stdout,"CREATEFRAME\n");
    create_frame();
    // Nahazet do TF
    fprintf(stdout,"PUSHFRAME\n");
    push_frame(frame_stack,NULL,0);
    char * cond = generate_expression(condition);
    fprintf(stdout, "JUMPIFNEQ %s bool@true LF@%s\n",end_label,cond);
    generate_program(state);
    fprintf(stdout,"POPFRAME\n");
    //Vyhazet z TF
    fprintf(stdout,"JUMP %s\nLABEL %s\n",label,end_label);
    fprintf(stdout,"POPFRAME\n");
    //Vyhazet z TF
    pop_frame(frame_stack);
}

char * generate_expression(ATLeaf *tree){
    char * id = generate_name(gt_variable);
    char * for_string;
    bool isString;
    GPStack * gp_stack = malloc(sizeof(struct GPStack));
    if(gp_stack == NULL){
        return NULL;
    }

    gsptr_stackInit(gp_stack);
    ATLeaf * current = tree;
    fprintf(stdout, "DEFVAR LF@%s\n", id);

    while(tree->processed != true){
        if((current->data.type == at_token) || (current->data.type == at_operators) || 
        (current->data.type == at_tsitem)){
            if(current->left == NULL && current->right == NULL){
                if(current->data.Atr.token->type == type_integer){//int
                    current->processed = true;
                    fprintf(stdout, "MOVE LF@%s int@%d\n", id,current->data.Atr.token->atribute.int_value);
                    return id;
                }
                else if(current->data.Atr.token->type == type_double){//double
                    current->processed = true;
                    fprintf(stdout, "MOVE LF@%s float@%f\n", id, current->data.Atr.token->atribute.double_value);
                    return id;                
                }
                else if(current->data.Atr.token->type == type_string){
                    current->processed = true;
                    for_string = get_string(current->data.Atr.token->atribute.int_value);
                    fprintf(stdout, "MOVE LF@%s string@%s\n", id, for_string);
                    return id;
                    isString = true;
                }
            }
            else if(current->left != NULL && current->left->processed == false){//mozna se bude dat zbavit NULL
                if(current->left->data.type == at_token){
                    if(current->left->data.Atr.token->type == type_integer){//int
                        current->left->processed = true;
                        fprintf(stdout, "MOVE LF@%s int@%d\n", id,current->left->data.Atr.token->atribute.int_value);
                        fprintf(stdout, "PUSHS LF@%s\n",id);
                    }
                    else if(current->left->data.Atr.token->type == type_double){//double
                        current->left->processed = true;
                        fprintf(stdout, "MOVE LF@%s float@%f\n", id, current->left->data.Atr.token->atribute.double_value);
                        fprintf(stdout, "PUSHS LF@%s\n", id);
                    }
                    else if(current->left->data.Atr.token->type == type_string){
                        current->left->processed = true;
                        for_string = get_string(current->left->data.Atr.token->atribute.int_value);
                        fprintf(stdout, "MOVE LF@%s string@%s\n", id, for_string);
                        fprintf(stdout, "PUSHS LF@%s\n", id);
                        isString = true;
                    }
                }
                else if(current->left->data.type == at_tsitem){
                    //pres globalni ramec
                }
                else if(current->left->data.type == at_operators){
                    gsptr_stackPush(gp_stack, current);
                    current = current->left;
                }
                else if(current->left->data.type == at_type_cast){
                    gsptr_stackPush(gp_stack, current);
                    current = current->left;
                }
            }
            else if(current->right != NULL && current->right->processed == false){
                if(current->right->data.type == at_token){
                    if(current->right->data.Atr.token->type == type_integer){//int
                        current->right->processed = true;
                        fprintf(stdout, "MOVE LF@%s int@%d\n", id,current->right->data.Atr.token->atribute.int_value);
                        fprintf(stdout, "PUSHS LF@%s\n",id);
                    }
                    else if(current->right->data.Atr.token->type == type_double){//double
                        current->right->processed = true;
                        fprintf(stdout, "MOVE LF@%s float@%f\n", id, current->right->data.Atr.token->atribute.double_value);
                        fprintf(stdout, "PUSHS LF@%s\n", id);
                    }
                    else if(current->right->data.Atr.token->type == type_string){
                        current->right->processed = true;
                        for_string = get_string(current->right->data.Atr.token->atribute.int_value);
                        fprintf(stdout, "MOVE LF@%s string@%s\n", id, for_string);
                        fprintf(stdout, "PUSHS LF@%s\n", id);
                        isString = true;                        
                    }
                }
                else if(current->right->data.type == at_tsitem){
                    //pres globalni ramec
                }
                else if(current->right->data.type == at_operators){ 
                    gsptr_stackPush(gp_stack, current);
                    current = current->right;
                }
                else if(current->right->data.type == at_type_cast){
                    gsptr_stackPush(gp_stack, current);
                    current = current->right;
                }
            }
            // XXXXX true and true XXXXXX
            else if(current->right->processed == true && current->right->processed == true){
                if(current->data.type == at_operators){
                    if(current->data.Atr.op_value == op_assign){
                        fprintf(stdout, "EQS\n");
                    }
                    else if(current->data.Atr.op_value == op_add){
                        if(isString){
                            char * help1 = generate_name(gt_variable);
                            char * help2 = generate_name(gt_variable);
                            fprintf(stdout, "DEFVAR LF@%s\n", help1);
                            fprintf(stdout, "DEFVAR LF@%s\n", help2);
                            fprintf(stdout,"POPS LF@%s\n",help1);
                            fprintf(stdout,"POPS LF@%s\n",help2); 
                            fprintf(stdout,"CONCAT LF@%s LF@%s LF@%s\n",id, help2, help1);
                            free(help1);
                            free(help2);
                            return id;
                            //pak se sem da dat return zatim to pushnu at se to popne
                            // fprintf(stdout,"PUSHS LF@%s\n", id);
                        }else{ 
                            fprintf(stdout, "ADDS\n");
                        }
                    }
                    else if(current->data.Atr.op_value == op_sub){
                        fprintf(stdout, "SUBS\n");
                    }
                    else if(current->data.Atr.op_value == op_mul){
                        fprintf(stdout, "MULS\n");
                    }
                    else if(current->data.Atr.op_value == op_division_int || current->data.Atr.op_value == op_slash){
                        fprintf(stdout, "DIVS\n");
                    }
                    //na podminky jak udelat je rovno?
                    else if(current->data.Atr.op_value == op_not_equal){
                        fprintf(stdout, "EQS\n");
                        fprintf(stdout, "NOTS\n");
                    }
                    else if(current->data.Atr.op_value == op_lesser){
                        fprintf(stdout, "LTS\n");
                    }
                    else if(current->data.Atr.op_value == op_greater){
                        fprintf(stdout, "GTS\n");
                    }
                    else if(current->data.Atr.op_value == op_lesser_equal ||
                    current->data.Atr.op_value == op_greater_equal){
                        char * help1 = generate_name(gt_variable);
                        char * help2 = generate_name(gt_variable);
                        fprintf(stdout, "DEFVAR LF@%s\n", help1);
                        fprintf(stdout, "DEFVAR LF@%s\n", help2);
                        fprintf(stdout,"POPS LF@%s\n",help1);
                        fprintf(stdout,"POPS LF@%s\n",help2);
                        fprintf(stdout,"PUSHS LF@%s\n",help2);
                        fprintf(stdout,"PUSHS LF@%s\n",help1);
                        fprintf(stdout,"PUSHS LF@%s\n",help2);
                        fprintf(stdout,"PUSHS LF@%s\n",help1);
                        if(current->data.Atr.op_value == op_lesser_equal){
                            fprintf(stdout, "LTS\n");
                            fprintf(stdout,"POPS LF@%s\n",help1);
                            fprintf(stdout, "EQS\n");
                            fprintf(stdout,"PUSHS LF@%s\n",help1);
                            fprintf(stdout, "ORS\n");
                        }
                        else if(current->data.Atr.op_value == op_greater_equal){
                            fprintf(stdout, "GTS\n");
                            fprintf(stdout,"POPS LF@%s\n",help1);
                            fprintf(stdout, "EQS\n");
                            fprintf(stdout,"PUSHS LF@%s\n",help1);
                            fprintf(stdout, "ORS\n");
                        }
                        free(help1);
                        free(help2);
                    }
                    //podminky pro if and while END
                    current->processed = true;
                    if(!(gsptr_stackEmpty(gp_stack))){
                        current = gsptr_stackTop(gp_stack);
                        gsptr_stackPop(gp_stack);
                    }
                }
            }
        }
        else if(current->data.type == at_type_cast){
            if(current->left == NULL){
                if(current->right->processed == true){
                    current->processed = true;
                    if(current->data.Atr.type_cast == 0){
                        fprintf(stdout, "INT2FLOATS\n");
                    }else if(current->data.Atr.type_cast == 1){
                        fprintf(stdout, "FLOAT2R2EINTS\n");
                    }
                }
                else if(current->right->data.type == at_token && current->right->processed == false){
                    current->right->processed = true;
                    fprintf(stdout, "MOVE LF@%s int@%d\n", id,current->right->data.Atr.token->atribute.int_value);
                    fprintf(stdout, "PUSHS LF@%s\n",id);
                }
                else if(current->right->data.type == at_tsitem && current->right->processed == false){

                }
                else if((current->right->data.type == at_operators || current->right->data.type == at_type_cast) 
                && (current->right->processed == false)){
                    gsptr_stackPush(gp_stack, current);
                    current = current->right;
                }
            }
            else if(current->right == NULL){
                if(current->left->processed == true){
                    if(current->data.Atr.type_cast == 0){
                        fprintf(stdout, "INT2FLOATS\n");
                    }else if(current->data.Atr.type_cast == 1){
                        fprintf(stdout, "FLOAT2R2EINTS\n");
                    }
                    current->processed = true;
                    if(!(gsptr_stackEmpty(gp_stack))){
                        current = gsptr_stackTop(gp_stack);
                        gsptr_stackPop(gp_stack);
                    }
                }
                else if(current->left->data.type == at_token && current->left->processed == false){
                    current->left->processed = true;
                    fprintf(stdout, "MOVE LF@%s int@%d\n", id,current->left->data.Atr.token->atribute.int_value);
                    fprintf(stdout, "PUSHS LF@%s\n",id);
                }
                else if(current->left->data.type == at_tsitem && current->left->processed == false){

                }
                else if((current->left->data.type == at_operators || current->left->data.type == at_type_cast) 
                && (current->left->processed == false)){
                    gsptr_stackPush(gp_stack, current);
                    current = current->left;
                }
            }
        }       
    }
    fprintf(stdout, "POPS LF@%s\n",id);
    gsptr_stackDestruct(gp_stack);;
    return id;
    fprintf(stdout, "WRITE LF@%s\n",id);

}
