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

//
typedef struct GPSVal{
    ATLeaf *leaf;
    struct GPSVal *next;
}GPSVal;

//
typedef struct GPStack{
    GPSVal* Top;
} GPStack;

/**
 * [gsptr_stackInit description]
 * @param s [description]
 */
void gsptr_stackInit(GPStack *s);

/**
 * [gsptr_stackPush description]
 * @param s    [description]
 * @param leaf [description]
 */
void gsptr_stackPush(GPStack* s, ATLeaf* leaf);

/**
 * [gsptr_stackEmpty description]
 * @param  s [description]
 * @return   [description]
 */
int gsptr_stackEmpty(const GPStack *s);

/**
 * [gsptr_stackTop description]
 * @param  s [description]
 * @return   [description]
 */
ATLeaf* gsptr_stackTop(const GPStack *s);

/**
 * [gsptr_stackPop description]
 * @param s [description]
 */
void gsptr_stackPop(GPStack *s);

/**
 * [gsptr_stackDestruct description]
 * @param s [description]
 */
void gsptr_stackDestruct(GPStack *s);

#endif
