/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */

#include "token.h"
#include "stack.h"
#include "destructor.h"
#include <stdlib.h>
#include <stdio.h>


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
            free(rm->Data);
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
    if(!stackEmpty(s))
        return s->Top->Data;
    else
        return NULL;
}

void stackPop ( tStack* s ){
    TSItem* rm = s->Top;
    s->Top = rm->Next;
    free(rm->Data);
    free(rm);
}

void stackPush ( tStack* s, SData* data ){
    TSItem* new_elem = (TSItem*) malloc(sizeof(struct SItem));
    if(new_elem == NULL){
        fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        dispose_global();
    }
    new_elem->Data = data;
    new_elem->Next = s->Top;
    s->Top = new_elem;
}

SData* getTerminalData(tStack* s){
    TSItem * current = s->Top;
    while(current != NULL){
        if(current->Data->Type == type_token){
            return current->Data;
        }
        current = current->Next;
    }
    return NULL;
}

void addHandler(tStack *s,SData * sData){
    TSItem * current = s->Top;
    TSItem * prev = NULL;
    if(sData == NULL){
        TSItem * newItem = malloc(sizeof(TSItem));
        if(newItem == NULL){
            fprintf(stderr, "%s\n", COMPILER_MESSAGE);
            dispose_global();
        }
        newItem->Data = malloc(sizeof(SData));
        if(newItem->Data == NULL){
            fprintf(stderr, "%s\n", COMPILER_MESSAGE);
            dispose_global();
        }
        newItem->Data->Type = type_handler;
        newItem->Next = NULL;
        if(stackEmpty(s)){
            s->Top = newItem;
        }
        else{
            while(current->Next != NULL){
                current = current->Next;
            }
            current->Next = newItem;
        }
    }
    else{
        while(current != NULL){
                if(current->Data == sData){
                    TSItem * newItem = malloc(sizeof(TSItem));
                    if(newItem == NULL){
                        fprintf(stderr, "%s\n", COMPILER_MESSAGE);
                        dispose_global();
                    }
                    if(prev != NULL)
                        prev->Next = newItem;
                    else
                        s->Top = newItem;
                    newItem->Next = current;
                    newItem->Data = malloc(sizeof(SData));
                    if(newItem->Data == NULL){
                        fprintf(stderr, "%s\n", COMPILER_MESSAGE);
                        dispose_global();
                    }
                    newItem->Data->Type = type_handler;
                    return;
                }
                prev = current;
                current = current->Next;
        }
    }
    
}