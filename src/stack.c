#include "token.h"
#include "stack.h"
#include <stdlib.h>


void stackInit ( tStack* s ){
    s->Top = NULL;
}

void stackDestruct ( tStack* s ){
    if(s != NULL){
        TSItem* current = s->Top;
        TSItem* rm = NULL;
        while(current != NULL){
            rm = current;
            current = current->Next;
            free(rm);
        }
        free(s);
        s = NULL;
    }
}

int stackEmpty ( const tStack* s ){
    return (s->Top == NULL);
}

SData* stackTop ( const tStack* s){
    return s->Top->Data;
}

void stackPop ( tStack* s ){
    TSItem* rm = s->Top;
    s->Top = rm->Next;
    free(rm);
}

void stackPush ( tStack* s, SData* data ){
    TSItem* new_elem = (TSItem*) malloc(sizeof(struct SItem));
    new_elem->Data = data;
    new_elem->Next = s->Top;
    s->Top = new_elem;
}

SData* getTerminalData(tStack* s){
    if(stackEmpty(s))
        return NULL;
    TSItem * current = s->Top;
    while(current->Data->Type == type_nonterm){
        current = current->Next;
        if(current == NULL)
            return NULL;
    }
    return current->Data;
}

void addHandler(SData * sData){
    if(sData != NULL){
        sData->Handler = 1;
    }
}