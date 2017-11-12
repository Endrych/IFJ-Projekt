#ifndef STACK_H
#define STACK_H

typedef enum{
    type_token,
    type_nonterm,
    type_handler
}SType;

typedef union{
    Token* Token;
    unsigned short Value;
}SAtribute;

typedef struct{
    SType Type;
    SAtribute Atr;
}SData;

typedef struct SItem{
    SData* Data;
    struct SItem* Next;
}TSItem;

typedef struct Stack{
    TSItem* Top;
} tStack;

void stackInit ( tStack* s );
void stackDestruct ( tStack* s );
int stackEmpty ( const tStack* s );
int stackFull ( const tStack* s );
SData* stackTop ( const tStack* s);
void stackPop ( tStack* s );
void stackPush ( tStack* s, SData* data);
SData* getTerminalData(tStack* s);
void addHandler(tStack* s,SData * sData);
#endif