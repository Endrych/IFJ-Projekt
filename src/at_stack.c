#include "at_stack.h"
#include <stdlib.h>

void listInit ( ATStack* s ){
    s->Head = NULL;
}

void listDestruct ( ATStack* s ){
    if(s != NULL){
        ATSItem* current = s->Head;
        ATSItem* rm = NULL;
        while(current != NULL){
            rm = current;
            current = current->Next;
            dispose_at(rm->Tree);
            free(rm);
        }
        free(s);
        s = NULL;
    }
}

int listEmpty ( const ATStack* s ){
    return (s->Head == NULL);
}

ATLeaf* listTop ( const ATStack* s){
    if(!listEmpty(s))
        return s->Head->Tree;
    else
        return NULL;
}

void stackPop ( ATStack* s ){
    ATSItem * rm = s->Top;
    s->Top = rm->Next;
    dispose_at(rm->Tree);
    free(rm);
}

void stackPush ( ATStack* s, ATLeaf* Tree){
    ATSItem* new_elem = (ATSItem*) malloc(sizeof(ATSItem));
    if(new_elem == NULL){
        return;
    }
    new_elem->Tree = Tree;
    new_elem->Next = s->Top;
    s->Top = new_elem;
}