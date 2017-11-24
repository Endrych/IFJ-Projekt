#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "symtable.h"
#include "token.h"
#include "frame.h"
#include "abstract_tree.h"
#include "generator.h"
#include "destructor.h"
#include "error.h"
#include "name_generator.h"
#include "gen_stacks.h"
#include "at_que.h"
#include "string_storage.h"

extern Tframe* temp_frame;
TFstack * frame_stack = NULL;

void generate_start(ATQueue *queue){
    frame_stack = malloc(sizeof(TFstack));
    if (frame_stack == NULL)
    {
        fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        dispose_global();
    }
    FS_init(frame_stack);
    create_frame();
    push_frame(frame_stack,NULL,0);
    fprintf(stdout,".IFJcode17\nJUMP $$main\n");
    open_output();
    generate_built_in();
    generate_program(queue);
    create_frame();
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
    create_frame();
    pop_frame(frame_stack);
}

void generate_variable_declaration(Tsymtab_item * id, ATLeaf * expr){
    fprintf(stdout, "DEFVAR LF@%s\n", id->key);
    if(expr != NULL){
        char *e = generate_expression(expr);
        fprintf(stdout, "MOVE LF@%s LF@%s\n", id->key, e);
    }
    else{
        if(id->type_strct.variable->type == type_int){
            fprintf(stdout, "MOVE LF@%s int@0\n",id->key);  
        }
        else if(id->type_strct.variable->type == type_doub){
            fprintf(stdout, "MOVE LF@%s float@0.0\n",id->key);  
        }
        else if(id->type_strct.variable->type == type_str){
            fprintf(stdout, "MOVE LF@%s string@\n",id->key);  
        }
    }
    Tvariable * item = malloc(sizeof(Tvariable));
    if (item == NULL)
    {
        fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        dispose_global();
    }
    item->id = id->key;
    item->type = id->type_strct.variable->type;
    Tframe *tmp = FS_top(frame_stack);
    add_var_to_frame(tmp,item);
}

void generate_assign(Tsymtab_item* id, ATLeaf * expr){
    char *e = generate_expression(expr);
    fprintf(stdout, "MOVE LF@%s LF@%s\n", id->key, e);
}

void generate_input(Tsymtab_item * id){
    if(id->type_strct.variable->type == type_int){
        fprintf(stdout, "READ LF@%s int\n",id->key);  
    }
    else if(id->type_strct.variable->type == type_doub){
        fprintf(stdout, "READ LF@%s float\n",id->key);  
    }
    else if(id->type_strct.variable->type == type_str){
        fprintf(stdout, "READ LF@%s string\n",id->key);  
    }
}

void generate_print(eQueue * exprs){
    eQueue * new = exprs;
    while(!equeEmpty(new)){
        char * expr = generate_expression(new->Front->eValue.tree_value);
        fprintf(stdout,"WRITE LF@%s\n",expr);
        equeRemove(new);
    }
}

void generate_call_function(Tsymtab_item * id, Tsymtab_item * sym_item, eQueue * param){
    param = param;
    fprintf(stdout,"CREATEFRAME\n");
    for(int i = 0; i < sym_item->type_strct.function->arg_count;i++){
        eQItem * par = equeFront(param);
        fprintf(stdout,"DEFVAR TF@%s\n",sym_item->type_strct.function->arguments[i].key);
        if(sym_item->type_strct.function->arguments[i].type_strct.variable->type == type_doub){
            if(par->etype == eq_id){
                fprintf(stdout,"MOVE TF@%s LF@%s\n",sym_item->type_strct.function->arguments[i].key,par->eValue.id_value->key);
                if(par->eValue.id_value->type_strct.variable->type == type_int)
                    fprintf(stdout,"INT2FLOAT TF@%s TF@%s\n",sym_item->type_strct.function->arguments[i].key,sym_item->type_strct.function->arguments[i].key);
            }
            else if(par->etype == eq_token){
                if(par->eValue.token_value->type == type_integer){
                    fprintf(stdout,"MOVE TF@%s float@%g\n",sym_item->type_strct.function->arguments[i].key,(double)par->eValue.token_value->atribute.int_value);
                }
                else if(par->eValue.token_value->type == type_double){
                    fprintf(stdout,"MOVE TF@%s float@%g\n",sym_item->type_strct.function->arguments[i].key,par->eValue.token_value->atribute.double_value);
                }
            }
        }
        else if(sym_item->type_strct.function->arguments[i].type_strct.variable->type == type_int){
            if(par->etype == eq_id){
                fprintf(stdout,"MOVE TF@%s LF@%s\n",sym_item->type_strct.function->arguments[i].key,par->eValue.id_value->key);
                if(par->eValue.id_value->type_strct.variable->type == type_doub)
                    fprintf(stdout,"FLOAT2R2EINT TF@%s TF@%s\n",sym_item->type_strct.function->arguments[i].key,sym_item->type_strct.function->arguments[i].key);
            }
            else if(par->etype == eq_token){
                if(par->eValue.token_value->type == type_integer){
                    fprintf(stdout,"MOVE TF@%s integer@%d\n",sym_item->type_strct.function->arguments[i].key,par->eValue.token_value->atribute.int_value);
                }
                else if(par->eValue.token_value->type == type_double){
                    fprintf(stdout,"MOVE TF@%s integer@%d\n",sym_item->type_strct.function->arguments[i].key,(int)par->eValue.token_value->atribute.double_value);
                }
            }
        }
        else if(sym_item->type_strct.function->arguments[i].type_strct.variable->type == type_str){
            if(par->etype == eq_id){
                fprintf(stdout,"MOVE TF@%s LF@%s\n",sym_item->type_strct.function->arguments[i].key,par->eValue.id_value->key);
            }
            else if(par->etype == eq_token){
                fprintf(stdout,"MOVE TF@%s string@%s\n",sym_item->type_strct.function->arguments[i].key,get_string(par->eValue.token_value->atribute.int_value));
            }  
        }
        //fprintf(stdout,"MOVE TF@%s LF@%s\n",sym_item->type_strct.function->arguments[i].key,prom);
        equeRemove(param);
    }
    fprintf(stdout,"CALL $%s\n",sym_item->key);
    fprintf(stdout,"MOVE LF@%s TF@%%retval\n",id->key);
}

void generate_return(Tsymtab_item * sym_item, PrecendentOutput * expr){
    char *e; //expression
    e = generate_expression(expr->Tree);
    if(sym_item->type_strct.function->return_type == type_int){
        if(expr->Type == type_int){
            fprintf(stdout, "MOVE LF@%%retval LF@%s\n", e);
        }
        else if(expr->Type == type_doub){
            fprintf(stdout, "MOVE LF@%%retval LF@%s\n", e);
            fprintf(stdout, "PUSHS LF@%%retval\n");
            fprintf(stdout, "FLOAT2R2EINTS\n");
            fprintf(stdout, "POPS LF@%%retval\n");
        }
    }
    else if(sym_item->type_strct.function->return_type == type_doub){
        if(expr->Type == type_doub){
            fprintf(stdout, "MOVE LF@%%retval LF@%s\n", e);
        }
        else if(expr->Type == type_int){
            fprintf(stdout, "MOVE LF@%%retval LF@%s\n", e);
            fprintf(stdout, "PUSHS LF@%%retval\n");
            fprintf(stdout, "INT2FLOATS\n");
            fprintf(stdout, "POPS LF@%%retval\n");
        }
    }
    else if(sym_item->type_strct.function->return_type == type_str){
        if(expr->Type == type_str){
            fprintf(stdout, "MOVE LF@%%retval LF@%s\n", e);
        }
    }
    fprintf(stdout, "JUMP $%s$epilog\n",sym_item->key);
}

void open_output(){
    
}

void generate_function(Tsymtab_item * item, ATQueue * state){
    fprintf(stdout, "LABEL $%s\n",item->key);
    create_frame();
    for(int i = 0; i < item->type_strct.function->arg_count;i++){
        Tvariable * item1 = malloc(sizeof(Tvariable));
        if (item1 == NULL)
        {
            fprintf(stderr, "%s\n", COMPILER_MESSAGE);
            dispose_global();
        }
        item1->id = item->type_strct.function->arguments[i].key;
        item1->type = item->type_strct.function->arguments[i].type;
        add_var_to_frame(temp_frame,item1);
    }
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
    create_frame();
    pop_frame(frame_stack);
}

void generate_if(ATLeaf * condition, ATQueue * state_true, ATQueue * state_false){
    char *label = generate_name(gt_label);
    char *cond = generate_expression(condition);
    char * end_label = generate_name(gt_label);
    fprintf(stdout, "JUMPIFNEQ %s bool@true LF@%s\n",label,cond);
    generate_program(state_true);
    fprintf(stdout,"JUMP %s\nLABEL %s\n",end_label,label);
    generate_program(state_false);
    fprintf(stdout,"LABEL %s\n",end_label);
}

void generate_while(ATLeaf * condition, ATQueue * state){
    char *label = generate_name(gt_label);
    char * end_label = generate_name(gt_label);
    fprintf(stdout,"LABEL %s\n",label);
    fprintf(stdout,"CREATEFRAME\n");
    Tframe * top_frame =  FS_top(frame_stack);
    for(int i= 0;i<top_frame->var_count;i++){
        fprintf(stdout,"DEFVAR TF@%s\n",top_frame->vars[i].id);
        fprintf(stdout,"MOVE TF@%s LF@%s\n",top_frame->vars[i].id,top_frame->vars[i].id);
    }
    fprintf(stdout,"PUSHFRAME\n");
    char * cond = generate_expression(condition);
    fprintf(stdout, "JUMPIFNEQ %s bool@true LF@%s\n",end_label,cond);
    generate_program(state);
    fprintf(stdout,"POPFRAME\n");   
    for(int i= 0;i<top_frame->var_count;i++){
        fprintf(stdout,"MOVE LF@%s TF@%s\n",top_frame->vars[i].id,top_frame->vars[i].id);
    }
    fprintf(stdout,"JUMP %s\nLABEL %s\n",label,end_label);
    fprintf(stdout,"POPFRAME\n");
}

char * generate_expression(ATLeaf *tree){
    char * id = generate_name(gt_variable);
    char * for_string;
    char * item_ts;
    bool isString;
    GPStack * gp_stack = malloc(sizeof(struct GPStack));
    if(gp_stack == NULL){
        fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        dispose_global();
    }

    gsptr_stackInit(gp_stack);
    ATLeaf * current = tree;
    fprintf(stdout, "DEFVAR LF@%s\n", id);

    while(tree->processed != true){
        if((current->data.type == at_token) || (current->data.type == at_operators) || 
        (current->data.type == at_tsitem)){
            if(current->left == NULL && current->right == NULL){
                if(current->data.type == at_token){
                    if(current->data.Atr.token->type == type_integer){//int
                        current->processed = true;
                        fprintf(stdout, "MOVE LF@%s int@%d\n", id,current->data.Atr.token->atribute.int_value);
                        return id;
                    }
                    else if(current->data.Atr.token->type == type_double){//double
                        current->processed = true;
                        fprintf(stdout, "MOVE LF@%s float@%g\n", id, current->data.Atr.token->atribute.double_value);
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
                else if(current->data.type == at_tsitem){
                    current->processed = true;
                    item_ts = current->data.Atr.tsItem->key;
                    fprintf(stdout, "MOVE LF@%s LF@%s\n",id, item_ts);
                    return id;
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
                        fprintf(stdout, "MOVE LF@%s float@%g\n", id, current->left->data.Atr.token->atribute.double_value);
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
                    current->left->processed = true;
                    item_ts = current->left->data.Atr.tsItem->key;
                    fprintf(stdout, "MOVE LF@%s LF@%s\n",id, item_ts);
                    fprintf(stdout, "PUSHS LF@%s\n", id);
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
                        fprintf(stdout, "MOVE LF@%s float@%g\n", id, current->right->data.Atr.token->atribute.double_value);
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
                    current->right->processed = true;
                    item_ts = current->right->data.Atr.tsItem->key;
                    fprintf(stdout, "MOVE LF@%s LF@%s\n",id, item_ts);
                    fprintf(stdout, "PUSHS LF@%s\n", id);
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
                    current->right->processed = true;
                    item_ts = current->right->data.Atr.tsItem->key;
                    fprintf(stdout, "MOVE LF@%s LF@%s\n",id, item_ts);
                    fprintf(stdout, "PUSHS LF@%s\n", id);
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
                    current->left->processed = true;
                    item_ts = current->left->data.Atr.tsItem->key;
                    fprintf(stdout, "MOVE LF@%s LF@%s\n",id, item_ts);
                    fprintf(stdout, "PUSHS LF@%s\n", id);
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
    // fprintf(stdout, "WRITE LF@%s\n",id);
}

void generate_Length()
{
    fprintf(stdout, "LABEL $Length\n");

    /*create_frame();

    for(int i = 0; i < item->type_strct.function->arg_count;i++){
        Tvariable * item1 = (Tvariable *) malloc(sizeof(Tvariable));
        if (item1 == NULL)
        {
            fprintf(stderr, "%s\n", COMPILER_MESSAGE);
            dispose_global();
        }

        item1->id = item->type_strct.function->arguments[i].key;
        item1->type = item->type_strct.function->arguments[i].type;

        add_var_to_frame(temp_frame,item1);
    }

    push_frame(frame_stack,NULL,0);*/
    fprintf(stdout, "PUSHFRAME\nDEFVAR LF@%%retval\n");

    fprintf(stdout, "MOVE LF@%%retval int@0\n");
    fprintf(stdout, "STRLEN LF@%%retval LF@s\n");

   // create_frame();
   // pop_frame(frame_stack)
    fprintf(stdout, "LABEL $Length$epilog\n");
    fprintf(stdout, "POPFRAME\nRETURN\n");
}


void generate_SubStr()
{
    fprintf(stdout, "LABEL $SubStr\n");
    fprintf(stdout, "PUSHFRAME\nDEFVAR LF@%%retval\n");
    fprintf(stdout, "MOVE LF@%%retval string@\n");

    fprintf(stdout, "DEFVAR LF@tmp1\nDEFVAR LF@tmp2\n");
    fprintf(stdout, "MOVE LF@tmp1 int@0\nMOVE LF@tmp2 string@\n");
    
    fprintf(stdout, "DEFVAR LF@length\n");
    fprintf(stdout, "MOVE LF@length int@0\n");
    
    fprintf(stdout, "DEFVAR LF@result\n");
    fprintf(stdout, "MOVE LF@result bool@false\n");

    fprintf(stdout, "JUMPIFEQ $SubStrReturn0 LF@s LF@tmp2\n");
    
    fprintf(stdout, "PUSHS LF@i\n");
    fprintf(stdout, "PUSHS int@0\n");
    fprintf(stdout, "PUSHS LF@i\n");
    fprintf(stdout, "PUSHS int@0\n");

    fprintf(stdout, "EQS\n");
    fprintf(stdout, "POPS LF@result\n");
    fprintf(stdout, "GTS\n");
    fprintf(stdout, "PUSHS LF@result\n");
    fprintf(stdout, "ORS\n");
    fprintf(stdout, "POPS LF@result\n");

    fprintf(stdout, "JUMPIFNEQ $SubStrReturn0 bool@true LF@result\n");

    fprintf(stdout, "PUSHS LF@n\n");
    fprintf(stdout, "PUSHS int@0\n");
    fprintf(stdout, "GTS\n");
    fprintf(stdout, "POPS LF@result\n");
    fprintf(stdout, "JUMPIFEQ $SubStrReturnRest bool@true LF@result\n");

    fprintf(stdout, "STRLEN LF@tmp1 LF@s\n");
    fprintf(stdout, "SUB LF@tmp1 LF@tmp1 LF@i\n");

    fprintf(stdout, "PUSHS LF@tmp1\n");
    fprintf(stdout, "PUSHS LF@n\n");
    fprintf(stdout, "GTS\n");
    fprintf(stdout, "POPS LF@result\n");

    fprintf(stdout, "JUMPIFEQ $SubStrReturnRest bool@true LF@result\n");

    fprintf(stdout, "DEFVAR LF@tmplen\n");
    fprintf(stdout, "MOVE LF@tmplen int@0\n");

    fprintf(stdout, "STRLEN LF@tmplen LF@s\n");
    fprintf(stdout, "SUB LF@tmplen LF@tmplen LF@i\n");

    fprintf(stdout, "SUB LF@tmp1 LF@i int@1\n");

    fprintf(stdout, "LABEL $SubStrFor\n");
    fprintf(stdout, "GETCHAR LF@tmp2 LF@s LF@tmp1\n");
    fprintf(stdout, "CONCAT LF@%%retval LF@%%retval LF@tmp2\n");
    fprintf(stdout, "ADD LF@tmp1 LF@tmp1 int@1\n");
    fprintf(stdout, "STRLEN LF@length LF@tmp2\n");
    fprintf(stdout, "JUMPIFNEQ $SubStrFor LF@length LF@n\n");
    fprintf(stdout, "JUMP $SubStr$epilog\n");
    
    fprintf(stdout, "LABEL $SubStrReturn0\n");
    fprintf(stdout, "MOVE LF@%%retval string@\n");
    fprintf(stdout, "JUMP $SubStr$epilog\n");
    fprintf(stdout, "LABEL $SubStrReturnRest\n");
    fprintf(stdout, "GETCHAR LF@tmp2 LF@s LF@tmp1\n");
    fprintf(stdout, "CONCAT LF@%%retval LF@%%retval LF@tmp2\n");
    fprintf(stdout, "ADD LF@tmp1 LF@tmp1 int@1\n");
    fprintf(stdout, "STRLEN LF@length LF@%%retval\n");
    fprintf(stdout, "JUMPIFNEQ $SubStrReturnRest LF@tmplen LF@length\n");

    fprintf(stdout, "LABEL $SubStr$epilog\n");
    fprintf(stdout, "POPFRAME\n");
    fprintf(stdout, "RETURN\n");
}

void generate_Asc()
{
    fprintf(stdout, "LABEL $Asc\n");
    fprintf(stdout, "PUSHFRAME\n");
    fprintf(stdout, "DEFVAR LF@%%retval\n");
    fprintf(stdout, "MOVE LF@%%retval int@0\n");

    fprintf(stdout, "DEFVAR LF@result\n");
    fprintf(stdout, "MOVE LF@result bool@false\n");

    fprintf(stdout, "DEFVAR LF@tmp\nDEFVAR LF@tmp2\n");
    fprintf(stdout, "MOVE LF@tmp string@\nMOVE LF@tmp2 int@0\n");

    fprintf(stdout, "PUSHS LF@i\n");
    fprintf(stdout, "PUSHS int@0\n");
    fprintf(stdout, "PUSHS LF@i\n");
    fprintf(stdout, "STRLEN LF@tmp2 LF@s\n");
    fprintf(stdout, "PUSHS LF@tmp2\n");
    fprintf(stdout, "LTS\n");
    fprintf(stdout, "POPS LF@result\n");
    fprintf(stdout, "GTS\n");
    fprintf(stdout, "PUSHS LF@result\n");
    fprintf(stdout, "ORS\n");
    fprintf(stdout, "POPS LF@result\n");
    fprintf(stdout, "JUMPIFNEQ $Asc$epilog bool@true LF@result\n");

    fprintf(stdout, "GETCHAR LF@tmp LF@s LF@i\n");
    fprintf(stdout, "STRI2INT LF@%%retval LF@tmp int@0\n");
    fprintf(stdout, "LABEL $Asc$epilog\n");
    fprintf(stdout, "POPFRAME\n");
    fprintf(stdout, "RETURN\n");
}

void generate_Chr()
{
    fprintf(stdout, "LABEL $Chr\n");
    fprintf(stdout, "PUSHFRAME\n");
    fprintf(stdout, "DEFVAR LF@%%retval\n");
    fprintf(stdout, "MOVE LF@%%retval string@\n");

    fprintf(stdout, "DEFVAR LF@tmp\n");
    fprintf(stdout, "MOVE LF@tmp string@\n");

    fprintf(stdout, "INT2CHAR LF@%%retval LF@i\n");

    fprintf(stdout, "POPFRAME\n");
    fprintf(stdout, "RETURN\n");



}

void generate_built_in()
{
    generate_Length();
    generate_SubStr();
    generate_Asc();
    generate_Chr();
}