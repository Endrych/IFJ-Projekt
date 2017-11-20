#include "gen_stacks.h"
#include <stdlib.h>

// xxxxxxxx VAL STACK xxxxxxxxx

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
        while(current != NULL){
            current = current->next;
            gsval_stackPop(s);
        }
        free(s);
        s = NULL;
    }
}

// xxxxxxxx PTR STACK xxxxxxxxx

void gsptr_stackInit(GPStack *s){
    s->Top = NULL;
}

void gsptr_stackPush(GPStack* s, ATLeaf* leaf){
    GPSVal* new_item = (GPSVal*) malloc(sizeof(struct GPSVal));
    if(leaf == NULL){
        return;
    }
    new_item->leaf = leaf;
    new_item->next = s->Top;
    s->Top = new_item;
}
//
int gsptr_stackEmpty(const GPStack *s){
    return(s->Top == NULL);
}

ATLeaf* gsptr_stackTop(const GPStack *s){
    if(!gsptr_stackEmpty(s)){
        return s->Top->leaf;
    }else{
        return NULL;
    }
}

void gsptr_stackPop(GPStack *s){
    GPSVal *rm = s->Top;
    s->Top = rm->next;
    free(rm);
}

void gsptr_stackDestruct(GPStack *s){
    if(s != NULL){
        GPSVal* current = s->Top;
        while(current != NULL){
            current = current->next;
            gsptr_stackPop(s);
        }
        free(s);
        s = NULL;
    }
}
