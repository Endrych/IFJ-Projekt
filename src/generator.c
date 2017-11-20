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

void open_output(){
    fprintf(stdout,".IFJcode17\n");
}

void generate_expression(ATLeaf *tree){
    char * id = generate_name(gt_variable);
    GVSData *new_data;  //free dole nekde
    GVSData *left_data;
    GVSData *right_data;    
    GVStack * gv_stack = malloc(sizeof(struct GVStack));
    GPStack * gp_stack = malloc(sizeof(struct GPStack));
    if(gp_stack == NULL){
        return NULL;
    }
    if(gv_stack == NULL){
        return;
    }
    gsptr_stackInit(gp_stack);
    gsval_init(gv_stack);
    ATLeaf * current = tree;

    // while(tree->processed != true){
        if(current->left != NULL && current->left->processed == false){
            if(current->left->data.type == at_token){
                if(current->left->data.Atr.token->type == type_integer){//int
                    current->left->processed = true;
                    new_data = malloc(sizeof(struct GVSData));
                    new_data->type = gvs_type_int;
                    new_data->value.int_value = current->left->data.Atr.token->atribute.int_value;
                    gsval_stackPush(gv_stack,new_data);
                }
                else if(current->left->data.Atr.token->type == type_double){//double
                    current->left->processed = true;
                    new_data = malloc(sizeof(struct GVSData));
                    new_data->type = gvs_type_double;
                    new_data->value.float_value = current->left->data.Atr.token->atribute.double_value;
                    gsval_stackPush(gv_stack,new_data);
                    // left_data = gsval_stackTop(gv_stack);                                    
                    // printf("%f\n",left_data->value.float_value);
                }
            }
            else if(current->left->data.type == at_tsitem){
                //pres globalni ramec
            }
            else if(current->left->data.type == at_type_cast){

            }
            else if(current->left->data.type == at_operators){

            }
            
            
        }
    // }
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