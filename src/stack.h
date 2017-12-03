/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */

#include "abstract_tree.h"
#include "symtable.h"

#ifndef STACK_H
#define STACK_H

// Typ polozky
typedef enum{
    type_token,
    type_nonterm,
    type_handler
}SType;

// Atribut pro ulozeni tokenu nebo ukazatele na ASS
typedef union{
    Token* Token;
    ATLeaf* Leaf;
}SAtribute;

// Data polozek zasobniku
// DataType Datovy typ polozky
// Type Typ polozky
// Atr Atribut polozky
typedef struct{
    Tvariable_type DataType;
    SType Type;
    SAtribute Atr;
}SData;

// Polozka zasobniku
// Data Data polozky zasobniku
// Next Ukazatel na nasledujici polozku
typedef struct SItem{
    SData* Data;
    struct SItem* Next;
}TSItem;

// Struktura zasobniku
// Top Ukazatel na vrchol zasobniku
typedef struct Stack{
    TSItem* Top;
} tStack;

/**
 * Funkce inicializuje zasobnik
 * @param s Ukazatel na zasobnik
 */
void stackInit ( tStack* s );

/**
 * Funkce pro zruseni zasobniku
 * @param s Ukazatel na zasobnik
 */
void stackDestruct ( tStack* s );

/**
 * Funkce pro zjisteni jesli je zasobnik prazdny
 * @param s Ukazatel na zasobnik
 * @return Vraci 1 pokud je zasobnik prazdny jinak vraci 0
 */
int stackEmpty ( const tStack* s );

/**
 * Funkce pro ziskani dat z vrcholu zasobniku
 * @param s Ukazatel na zasobnik
 * @return Vraci data z vrcholu zasobniku
 */
SData* stackTop ( const tStack* s);

/**
 * Funkce odebere polozku z vrcholu zasobniku
 * @param s Ukazatel na zasobnik
 */
void stackPop ( tStack* s );

/**
 * Funkce vlozi polozku do zasobniku
 * @param s Ukazatel na zasobnik
 * @param data Data, ktera se maji v zasobniku ulozit
 */
void stackPush ( tStack* s, SData* data);

/**
 * Funkce vraci ze zasobniku data prvnihu terminalu
 * @param s Ukazatel na zasobnik
 * @return Data terminalu
 */
SData* getTerminalData(tStack* s);

/**
 * Funkce vytvori v zasobniku zarazku na urcite pozici
 * @param s Ukazatel na zasobnik
 * @param sData Ukazatel na data v zasobniku, kde se zarazka ma vytvorit
 */
void addHandler(tStack* s,SData * sData);
#endif
