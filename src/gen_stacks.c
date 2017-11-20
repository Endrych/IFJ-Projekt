#include "gen_stacks.h"
#include <stdlib.h>

void gsval_init(GVStack *s){
    s->Top = NULL;
}

void gsval_stackPush(GVStack* s, GSVData* data){
    GVSVal* new_elem = (GVSVal*) malloc(sizeof(struct GVSVal));
    if(new_elem == NULL){
        return;
    }
    new_elem->data = data;
    new_elem->next = s->Top;
    s->Top = new_elem;
}

// void gsval_stackTop(const GVStack *s)