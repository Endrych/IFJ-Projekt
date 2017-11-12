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
   
    printf("2) Nonterminal: ");
    SData data1;
    data1.Type = type_nonterm;
    data1.Atr.Value = 0;
    stackPush(s,&data1);
    SData * test1 = stackTop(s);
    if(test1->Type == type_nonterm && test1->Atr.Value == 0)
        printf("Correct\n");

    printf("3) Get terminal: ");
    SData * d1 = getTerminalData(s);
    Token* t1 = d1->Atr.Token;
    if(t1 == integer_token)
        printf("Correct\n");

    printf("4) Pop, integer next: ");
    stackPop(s);
    SData * test2 = stackTop(s);
    if(test2->Type == type_token && test2->Atr.Token == integer_token)
        printf("Correct\n");

    printf("3) Get terminal which is second: ");
    d1 = getTerminalData(s);
    t1 = d1->Atr.Token;
    if(t1 == integer_token)
        printf("Correct\n");

    printf("3) Add handler: ");
    addHandler(s,d1);
    TSItem *curr = s->Top;
    while(curr != NULL){
        if(curr->Data->Type == type_handler){
            printf("Correct\n");
            break;
        }
        curr = curr->Next;
    }

    if(!stackEmpty(s))
        printf("Zasobnik neni prazdny\n");
    stackPop(s);
    stackPop(s);
    printf("Stack pop\n");
    if(stackEmpty(s))
        printf("Zasobnik je prazdny\n");
    

    printf("5) Get terminal in empty stack: ");
    d1 = getTerminalData(s);
    if(d1 != NULL)
        t1 = d1->Atr.Token;
    else 
        t1 = NULL;
    if(t1 == NULL)
        printf("Correct\n");
    stackPush(s,test1);
    printf("5) Get terminal without term stack: ");
    d1 = getTerminalData(s);
    if(d1 != NULL)
        t1 = d1->Atr.Token;
    else 
        t1 = NULL;
    if(t1 == NULL)
        printf("Correct\n");
    stackPush(s,test);
    printf("\n\n");
    stackDestruct(s);
    destruct_token(integer_token);
    return 0;
}