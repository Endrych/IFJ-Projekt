/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */


#include "gen_stacks.h"
#include "destructor.h"
#include <stdlib.h>
#include <stdio.h>

// xxxxxxxx PTR STACK xxxxxxxxx

void gsptr_stackInit(GPStack *s){
    s->Top = NULL;
}

void gsptr_stackPush(GPStack* s, ATLeaf* leaf){
    GPSVal* new_item = (GPSVal*) malloc(sizeof(struct GPSVal));
    if(leaf == NULL){
        fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        dispose_global();
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
