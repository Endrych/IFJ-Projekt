#ifndef STACK_H
#define STACK_H

typedef struct SItem{
    Token* Token;
    struct SItem* Next;
}TSItem;

typedef struct Stack{
    TSItem* Top;
} tStack;

void stackInit ( tStack* s );
void stackDestruct ( tStack* s );
int stackEmpty ( const tStack* s );
int stackFull ( const tStack* s );
Token* stackTop ( const tStack* s);
void stackPop ( tStack* s );
void stackPush ( tStack* s, Token* token );

#endif