#include "token.h"
#include "precedence_sa.h"
#include "lexical.h"
#include "stack.h"
#include <stdlib.h>

PrecendentOutput * precedence_analysis(Token* last_token, int * ptr){
    ptr = ptr; // stromecek
    int correct = 0;
    tStack *s = (tStack*) malloc(sizeof(struct Stack));
    if(s == NULL){
        return NULL;
    }
    stackInit(s);
    Token * current = last_token;
    if(current == NULL)
        current = get_token();
    
    while(!correct){
        Token* token = getTerminal(s)->Atr.Token;
        
        int operation = precedence_operation(token,current);
        if(operation == -1)
            break;
        else if(operation == 0){
            SData *data = malloc(sizeof(SData));
            if(data == NULL){
                return NULL;
            }
            data->Type = type_token;
            data->Atr.Token = current;
            stackPush(s,data);
        }
        else if(operation == 1){
            
        }
        else if(operation == 2){

        }
        token = get_token();
    }
    PrecendentOutput * out = malloc(sizeof(PrecendentOutput));
    if(out == NULL)
        return NULL;
    out->ReturnToken = current;
    out->Pntr = 1;
    return out;
}

int precedence_operation(Token* stack_token,Token* lexical_token){
    int index0,index1 = 0;
    Token * token = NULL;
    for(int i=0;i<2;i++){
        int curr_index = 0;
        if(i == 0){
            token = stack_token;
        }
        else{
            token = lexical_token;
        }
        if(token == NULL){
            curr_index = 14;            
        }
        else if(token->type == type_operator){
            if(token->atribute.operator_value == op_add){
                curr_index = 0;
            }
            else if(token->atribute.operator_value == op_sub){
                curr_index = 1;
            }
            else if(token->atribute.operator_value == op_mul){
                curr_index = 2;
            }
            else if(token->atribute.operator_value == op_slash){
                curr_index = 3;
            }
            else if(token->atribute.operator_value == op_division_int){
                curr_index = 4;
            }
            else if(token->atribute.operator_value == op_assign){
                curr_index = 5;                    
            }
            else if(token->atribute.operator_value == op_not_equal){
                curr_index = 6;
            }
            else if(token->atribute.operator_value == op_lesser){
                curr_index = 7;
            }
            else if(token->atribute.operator_value == op_greater){
                curr_index = 8;
            }
            else if(token->atribute.operator_value == op_lesser_equal){
                curr_index = 9;
            }
            else if(token->atribute.operator_value == op_greater_equal){
                curr_index = 10;
            }
            else if(token->atribute.operator_value == op_bracket){
                curr_index = 11;
            }
            else if(token->atribute.operator_value == op_bracket_end){
                curr_index = 12;
            }
            else{
                curr_index = -1;
            }
        }
        else if(token->type == type_id || token->type == type_integer || token->type == type_string || token->type == type_double){
            curr_index = 13;        
        }
        else{
            curr_index = -1;
        }
        if(i == 0){
            index0 = curr_index;
        }
        else{
            index1 = curr_index;
        }
    }
    static int precedence_table[15][15] = {  
                                            {2,2,1,1,1,2,2,2,2,2,2,1,2,1,2},
                                            {2,2,1,1,1,2,2,2,2,2,2,1,2,1,2},
                                            {2,2,2,2,2,2,2,2,2,2,2,1,2,1,2},
                                            {2,2,2,2,2,2,2,2,2,2,2,1,2,1,2},
                                            {2,2,2,2,2,2,2,2,2,2,2,1,2,1,2},
                                            {2,2,2,2,2,2,2,2,2,2,2,1,2,1,2},
                                            {1,1,1,1,1,-1,-1,-1,-1,-1,-1,1,2,1,2},
                                            {1,1,1,1,1,-1,-1,-1,-1,-1,-1,1,2,1,2},
                                            {1,1,1,1,1,-1,-1,-1,-1,-1,-1,1,2,1,2},
                                            {1,1,1,1,1,-1,-1,-1,-1,-1,-1,1,2,1,2},
                                            {1,1,1,1,1,-1,-1,-1,-1,-1,-1,1,2,1,2},
                                            {1,1,1,1,1,-1,-1,-1,-1,-1,-1,1,2,1,2},
                                            {1,1,1,1,1,1,1,1,1,1,1,1,0,1,-1},
                                            {2,2,2,2,2,2,2,2,2,2,2,-1,2,-1,2},
                                            {2,2,2,2,2,2,2,2,2,2,2,-1,2,-1,2},
                                            {1,1,1,1,1,1,1,1,1,1,1,1,-1,1,-1},
                                        };
    return precedence_table[index0][index1];
}