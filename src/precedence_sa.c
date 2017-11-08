#include "token.h"
#include "precedence_sa.h"
#include "lexical.h"
#include "stack.h"
#include <stdlib.h>

PrecendentOutput * precedence_analysis(Token* token, int * ptr){
    ptr = ptr; // stromecek
    int correct = 0;
    tStack *s = (tStack*) malloc(sizeof(struct Stack));
    if(s == NULL){
        return NULL;
    }
    stackInit(s);
    Token * current = token;
    if(current == NULL)
        token = get_token();
    
    while((token->type != type_keyword && token->atribute.keyword_value != kw_then)||token->type != type_eol || token->type != type_semicolon){
        Token* token = getTerminal(s);
        if(token == NULL){

        }
        else if(token->type == type_operator){
            if(token->atribute.operator_value == op_add){

            }
            else if(token->atribute.operator_value == op_sub){

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
    out->ReturnToken = token;
    out->Pntr = correct;
    return out;
}