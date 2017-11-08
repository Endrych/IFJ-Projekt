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
        Token* token = getTerminal(s);
        if(token == NULL){

        }
        else if(token->type == type_operator){
            if(token->atribute.operator_value == op_add || token->atribute.operator_value == op_sub){
                if((current->type == type_operator && (current->atribute.operator_value == op_mul || current->atribute.operator_value == op_slash || current->atribute.operator_value == op_division_int || current->atribute.operator_value == op_bracket)) || (token->type == type_id || token->type == type_integer || token->type == type_string || token->type == type_double)){

                }
                else if((current->type == type_operator && (current->atribute.operator_value == op_add || current->atribute.operator_value == op_sub || current->atribute.operator_value == op_assign || current->atribute.operator_value == op_not_equal || current->atribute.operator_value == op_lesser || current->atribute.operator_value == op_greater || current->atribute.operator_value == op_greater_equal || current->atribute.operator_value == op_lesser_equal || current->atribute.operator_value == op_bracket_end)) || ((current->type != type_keyword && current->atribute.keyword_value != kw_then)||(current->type != type_eol || current->type != type_semicolon))){

                }
            }
            else if(token->atribute.operator_value == op_mul){
                
            }
            else if(token->atribute.operator_value == op_slash){
                
            }
            else if(token->atribute.operator_value == op_assign){
                
            }
            else if(token->atribute.operator_value == op_not_equal){
                
            }
            else if(token->atribute.operator_value == op_not_equal){
                
            }
            else if(token->atribute.operator_value == op_lesser){
                
            }
            else if(token->atribute.operator_value == op_greater){
                
            }
            else if(token->atribute.operator_value == op_lesser_equal){
                
            }
            else if(token->atribute.operator_value == op_greater_equal){
                
            }
            else if(token->atribute.operator_value == op_bracket){
                
            }
            else if(token->atribute.operator_value == op_bracket_end){
                
            }
            else{

            }
        }
        else if(token->type == type_id || token->type == type_integer || token->type == type_string || token->type == type_double){

        }
        else{

        }
        token = get_token();
    }
    PrecendentOutput * out = malloc(sizeof(PrecendentOutput));
    if(out == NULL)
        return NULL;
    out->ReturnToken = current;
    out->Pntr = correct;
    return out;
}