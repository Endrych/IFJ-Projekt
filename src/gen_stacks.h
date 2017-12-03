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
 * Nastavi vrchol zasobniku na NULL
 *  @param s ukazatel na zasobnik
 */
void gsptr_stackInit(GPStack *s);

/**
 * Vlozi na vrchol zasobnik ukazatel na uzel abstraktniho stromu
 * @param s    ukazatel na zasobnik
 * @param leaf ukazatel na uzel abstraktniho stromu
 */
void gsptr_stackPush(GPStack* s, ATLeaf* leaf);

/**
 * Provede kontrolu zda je zasobnik prazdny
 * @param  s ukazatel na zasobnik
 * @return vrati true pokud je zasobnik prazdny jinak vrati false
 */
int gsptr_stackEmpty(const GPStack *s);

/**
 * Vraci ukazatel uzel abstraktniho stromu na vrcholu zasobniku
 * @param  s ukazatel na zasobnik
 * @return   ukazatel na uzel abstraktniho stromu
 */
ATLeaf* gsptr_stackTop(const GPStack *s);

/**
 * Odstrani hodnotu z vrcholu zasobniku a provede jeji uvolneni
 * @param s ukazatel na zasobnik
 */
void gsptr_stackPop(GPStack *s);

/**
 * Uvolni vsechny hodnoty ze zasobniku
 * @param s ukazatel na zasobnik
 */
void gsptr_stackDestruct(GPStack *s);

#endif
