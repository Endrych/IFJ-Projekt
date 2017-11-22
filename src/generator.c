#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "symtable.h"
#include "token.h"
// #include "frame.h"
#include "abstract_tree.h"
#include "generator.h"
#include "error.h"
#include "name_generator.h"
#include "gen_stacks.h"
#include "at_que.h"
#include "string_storage.h"

void open_output(){
    fprintf(stdout,".IFJcode17\n");
}

void generate_function(Tsymtab_item * item, ATQueue * state){
    fprintf(stdout, "LABEL $%s\n",item->key);
    fprintf(stdout, "PUSHFRAME\nDEFVAR LF@%%retval\n");
    Tfunction_item *  function = item->type_strct.function;
    if(function->return_type == type_doub){
        fprintf(stderr,"MOVE LF@%%retval float@0.0\n");
    }
    else if(function->return_type == type_int){
        fprintf(stderr,"MOVE LF@%%retval int@0\n");
    }
    else if(function->return_type == type_str){
        fprintf(stderr,"MOVE LF@%%retval string@\n");
    }
    while(!queEmpty(state)){
        generate_expression(queFront(state));
        queRemove(state);
    }
    fprintf(stdout, "LABEL $%s$epilog\n",item->key);
    fprintf(stdout, "POPFRAME\nRETURN\n");
}

void generate_if(ATLeaf * condition, ATQueue * state_true, ATQueue * state_false){
    char *label = generate_name(gt_label);
    generate_condition(condition,label);
    while(!queEmpty(state_true)){
        generate_expression(queFront(state_true));
        queRemove(state_true);
    }
    char * end_label = generate_name(gt_label);
    fprintf(stdout,"JUMP %s\nLABEL %s\n",end_label,label);
    while(!queEmpty(state_false)){
        generate_expression(queFront(state_false));
        queRemove(state_false);
    }
    fprintf(stdout,"LABEL %s\n",end_label);
}

void generate_while(ATLeaf * condition, ATQueue * state){
    char *label = generate_name(gt_label);
    char * end_label = generate_name(gt_label);
    fprintf(stdout,"LABEL %s\n",label);
    generate_condition(condition,end_label);
    while(!queEmpty(state)){
        generate_expression(queFront(state));
        queRemove(state);
    }
    fprintf(stdout,"JUMP %s\nLABEL %s\n",label,end_label);
}

char * generate_expression(ATLeaf *tree){
    char * id = generate_name(gt_variable);
    char * for_string;
    bool isString;
    GPStack * gp_stack = malloc(sizeof(struct GPStack));
    if(gp_stack == NULL){
        return;
    }

    gsptr_stackInit(gp_stack);
    ATLeaf * current = tree;
    fprintf(stdout, "DEFVAR GF@%s\n", id);

    while(tree->processed != true){
        if((current->data.type == at_token) || (current->data.type == at_operators) || 
        (current->data.type == at_tsitem)){
            if(current->left != NULL && current->left->processed == false){//mozna se bude dat zbavit NULL
                if(current->left->data.type == at_token){
                    if(current->left->data.Atr.token->type == type_integer){//int
                        current->left->processed = true;
                        fprintf(stdout, "MOVE GF@%s int@%d\n", id,current->left->data.Atr.token->atribute.int_value);
                        fprintf(stdout, "PUSHS GF@%s\n",id);
                    }
                    else if(current->left->data.Atr.token->type == type_double){//double
                        current->left->processed = true;
                        fprintf(stdout, "MOVE GF@%s float@%f\n", id, current->left->data.Atr.token->atribute.double_value);
                        fprintf(stdout, "PUSHS GF@%s\n", id);
                    }
                    else if(current->left->data.Atr.token->type == type_string){
                        current->left->processed = true;
                        for_string = get_string(current->left->data.Atr.token->atribute.int_value);
                        fprintf(stdout, "MOVE GF@%s string@%s\n", id, for_string);
                        fprintf(stdout, "PUSHS GF@%s\n", id);
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
                        fprintf(stdout, "MOVE GF@%s int@%d\n", id,current->right->data.Atr.token->atribute.int_value);
                        fprintf(stdout, "PUSHS GF@%s\n",id);
                    }
                    else if(current->right->data.Atr.token->type == type_double){//double
                        current->right->processed = true;
                        fprintf(stdout, "MOVE GF@%s float@%f\n", id, current->right->data.Atr.token->atribute.double_value);
                        fprintf(stdout, "PUSHS GF@%s\n", id);
                    }
                    else if(current->right->data.Atr.token->type == type_string){
                        current->right->processed = true;
                        for_string = get_string(current->right->data.Atr.token->atribute.int_value);
                        fprintf(stdout, "MOVE GF@%s string@%s\n", id, for_string);
                        fprintf(stdout, "PUSHS GF@%s\n", id);
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
                            fprintf(stdout, "DEFVAR GF@%s\n", help1);
                            fprintf(stdout, "DEFVAR GF@%s\n", help2);
                            fprintf(stdout,"POPS GF@%s\n",help1);
                            fprintf(stdout,"POPS GF@%s\n",help2); 
                            fprintf(stdout,"CONCAT GF@%s GF@%s GF@%s\n",id, help2, help1);
                            free(help1);
                            free(help2);
                            return id;
                            //pak se sem da dat return zatim to pushnu at se to popne
                            // fprintf(stdout,"PUSHS GF@%s\n", id);
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
                        fprintf(stdout, "DEFVAR GF@%s\n", help1);
                        fprintf(stdout, "DEFVAR GF@%s\n", help2);
                        fprintf(stdout,"POPS GF@%s\n",help1);
                        fprintf(stdout,"POPS GF@%s\n",help2);
                        fprintf(stdout,"PUSHS GF@%s\n",help2);
                        fprintf(stdout,"PUSHS GF@%s\n",help1);
                        fprintf(stdout,"PUSHS GF@%s\n",help2);
                        fprintf(stdout,"PUSHS GF@%s\n",help1);
                        if(current->data.Atr.op_value == op_lesser_equal){
                            fprintf(stdout, "LTS\n");
                            fprintf(stdout,"POPS GF@%s\n",help1);
                            fprintf(stdout, "EQS\n");
                            fprintf(stdout,"PUSHS GF@%s\n",help1);
                            fprintf(stdout, "ORS\n");
                        }
                        else if(current->data.Atr.op_value == op_greater_equal){
                            fprintf(stdout, "GTS\n");
                            fprintf(stdout,"POPS GF@%s\n",help1);
                            fprintf(stdout, "EQS\n");
                            fprintf(stdout,"PUSHS GF@%s\n",help1);
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
                    fprintf(stdout, "MOVE GF@%s int@%d\n", id,current->right->data.Atr.token->atribute.int_value);
                    fprintf(stdout, "PUSHS GF@%s\n",id);
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
                    fprintf(stdout, "MOVE GF@%s int@%d\n", id,current->left->data.Atr.token->atribute.int_value);
                    fprintf(stdout, "PUSHS GF@%s\n",id);
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
    fprintf(stdout, "POPS GF@%s\n",id);
    gsptr_stackDestruct(gp_stack);;
    return id;
    // fprintf(stdout, "WRITE GF@%s\n",id);

}

void generate_condition(ATLeaf *leaf, char* label){
    fprintf(stdout,"GENERATE CONDITION\n");
}


void to_print(ATLeaf *leaf, int value){
    if(leaf->left != NULL){
        to_print(leaf->left, 2);
    }
    if(leaf->data.type == 1){
        if(value == 1){
            printf("|%d|\n",leaf->data.Atr.token->atribute.int_value);        
        }else if(value == 2){
            printf("/%d/\n",leaf->data.Atr.token->atribute.int_value);                
        }else{
            printf("\\%d\\\n",leaf->data.Atr.token->atribute.int_value);                
        }
        if(leaf->right != NULL){
            to_print(leaf->right, 3);
        }
    }
    else if(leaf->data.type == 0){
         if(value == 1){
            printf("|%d op|\n",leaf->data.Atr.op_value);        
        }else if(value == 2){
            printf("/%d op/\n",leaf->data.Atr.op_value);                
        }else{
            printf("\\%d op\\\n",leaf->data.Atr.op_value);                
        }
        if(leaf->right != NULL){
            to_print(leaf->right, 3);
        }
    }else if(leaf->data.type == 2){
        if(value == 1){
            printf("|%s|\n",leaf->data.Atr.tsItem->key);        
        }else if(value == 2){
            printf("/%s/\n",leaf->data.Atr.tsItem->key);                
        }else{
            printf("\\%s\\\n",leaf->data.Atr.tsItem->key);                
        }
        if(leaf->right != NULL){
            to_print(leaf->right, 3);
        }
    }

    
}