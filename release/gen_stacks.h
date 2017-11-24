/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */


#include "abstract_tree.h"

#ifndef GEN_STACK_H
#define GEN_STACK_H

// xxxxxxxx PTR STACK xxxxxxxxx

typedef struct GPSVal{
    ATLeaf *leaf;
    struct GPSVal *next;
}GPSVal;

typedef struct GPStack{
    GPSVal* Top;
} GPStack;


void gsptr_stackInit(GPStack *s);
void gsptr_stackPush(GPStack* s, ATLeaf* leaf);
int gsptr_stackEmpty(const GPStack *s);
ATLeaf* gsptr_stackTop(const GPStack *s);
void gsptr_stackPop(GPStack *s);
void gsptr_stackDestruct(GPStack *s);

#endif