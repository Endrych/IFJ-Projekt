#include "at_que.h"
#include <stdlib.h>

void queInit ( ATQueue* s ){
    s->Front = NULL;
}

void queDestruct ( ATQueue* s ){
    if(s != NULL){
        ATQItem* current = s->Front;
        ATQItem* rm = NULL;
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

int queEmpty ( const ATQueue* s ){
    return (s->Front == NULL);
}

ATLeaf* queFront ( const ATQueue* s){
    if(!queEmpty(s))
        return s->Front->Tree;
    else
        return NULL;
}

void queRemove ( ATQueue* s ){
    ATQItem * rm = s->Front;
    s->Front = rm->Next;
    dispose_at(rm->Tree);
    free(rm);
}

void queUp ( ATQueue* s, ATLeaf* Tree){
    ATQItem* new_elem = (ATQItem*) malloc(sizeof(ATQItem));
    if(new_elem == NULL){
        return;
    }
    new_elem->Tree = Tree;
    new_elem->Next = NULL;
    ATQItem * current = s->Front;
    if(queEmpty(s)){
        s->Front = new_elem;
        return;
    }
    while(current->Next != NULL){
        current = current->Next;
    }
    current->Next = new_elem;
}