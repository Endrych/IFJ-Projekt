#include <stdio.h>
#include <stdlib.h>
#include "../src/token.h"
#include "../src/stack.h"

int main()
{
    tStack *s = (tStack*) malloc(sizeof(struct Stack));
    stackInit(s);
    Token* integer_token = create_token();
    printf("\n\n\n\n");
    integer_token->type = type_integer;
    integer_token->atribute.int_value = 15;
    stackPush(s, integer_token);
    Token *test = stackTop(s);
    if(test == integer_token)
        printf("Correct\n");
    else
        printf("Wrong \n");

    printf("\n\n\n\n");
    stackDestruct(s);
    destruct_token(integer_token);
    return 0;
}