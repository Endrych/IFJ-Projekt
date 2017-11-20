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

int gsval_stackEmpty(const GVStack *s){
    return(s->Top == NULL);
}

GSVData* gsval_stackTop(const GVStack *s){
    if(!gsval_stackEmpty(s)){
        return s->Top->data;
    }else{
        return NULL;
    }
}

void gsval_stackPop(GVStack *s){
    GVSVal *rm = s->Top;
    s->Top = rm->next;
    free(rm);
}

int gsval_stackCount(GVStack *s){
    int number_of_items = 0;
    GVSVal *count = s->Top;
    while(count != NULL){
        if(count->data != NULL){
            number_of_items++;
            if(count->next != NULL){
                count = count->next;
            }else{
                count = NULL;
            }
        }
    }
    return number_of_items;
}

void gsval_stackDestruct(GVStack *s){
    if(s != NULL){
        GVSVal* current = s->Top;
        GVSVal* rm = NULL;
        while(current != NULL){
            rm = current;
            current = current->next;
            gsval_stackPop(s);
        }
        free(s);
        s = NULL;
    }
}
