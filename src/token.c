#include "token.h"
#include <stdlib.h>

Token* create_token()
{
    Token* token;
    token = (Token*)malloc(sizeof(Token));
    return token;
}

void destruct_token(Token* token)
{
    free(token);
    token = NULL;
}