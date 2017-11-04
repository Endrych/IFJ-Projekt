#include <stdio.h>
#include <stdlib.h>
#include "../src/token.h"
#include "../src/stack.h"

int main()
{
    tStack *s = (tStack*) malloc(sizeof(struct Stack));
    stackInit(s);
    printf("\n\n");
    
    printf("1) Integer: ");
    Token* integer_token = create_token();
    integer_token->type = type_integer;
    integer_token->atribute.int_value = 15;

    SData data;
    data.Type = type_token;
    data.Atr.Token = integer_token;
    
    stackPush(s, &data);
    SData *test = stackTop(s);
    if(test->Type == type_token && test->Atr.Token == integer_token)
        printf("Correct\n");
    else
        printf("Wrong \n");

    
    printf("2) Nonterminal: ");
    SData data1;
    data1.Type = type_nonterm;
    data1.Atr.Value = 0;
    stackPush(s,&data1);
    SData * test1 = stackTop(s);
    if(test1->Type == type_nonterm && test1->Atr.Value == 0)
        printf("Correct\n");
    else
        printf("Wrong \n");

    printf("\n\n");
    stackDestruct(s);
    destruct_token(integer_token);
    return 0;
}