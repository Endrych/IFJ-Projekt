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
#include "gen_stack.h"

void open_output(){
    fprintf(stdout,".IFJcode17\n");
}

void generate_expression(ATLeaf *leaf){
    char * id = generate_name(gt_variable);
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
    ATLeaf * current = leaf;
    while(leaf->processed != true){
        if(current->left != NULL){
            if(current->data.type == type_operator){
                gsval_stackPush(gv_stack, )
            }
        }
    }
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

void generate_expression(ATLeaf *tree){

}
