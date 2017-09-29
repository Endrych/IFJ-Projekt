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

int is_keyword(char* text, int length)
{
    switch(length)
    {
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        default:
            return -1;
            break;
    }
}