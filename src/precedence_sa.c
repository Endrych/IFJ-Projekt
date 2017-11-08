#include "token.h"
#include "precedence_sa.h"
#include "lexical.h"

PrecendentOutput precedence_analysis(Token* token, int * ptr){
    Token * current = token;
    if(current == null)
        token = get_token();
    
    while((token->type != type_keyword && token.atribute != kw_then)||token->type != type_eol || token->type != type_semicolon){
        
    }
}