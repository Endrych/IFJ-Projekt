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
            current = current->Next;
            rm = current;
            free(rm);
        }
        free(s);
    }
}
int stackEmpty ( const tStack* s ){
    return (s->Top == NULL);
}
Token* stackTop ( const tStack* s){
    return s->Top->Token;
}
void stackPop ( tStack* s ){
    TSItem* rm = s->Top;
    s->Top = rm->Next;
    free(rm);
}
void stackPush ( tStack* s, Token* token ){
    TSItem* new_elem = (TSItem*) malloc(sizeof(struct SItem));
    new_elem->Token = token;
    new_elem->Next = s->Top;
    s->Top = new_elem;
}