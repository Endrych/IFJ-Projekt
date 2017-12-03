/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */

#include "token.h"
#include "symtable.h"
#include <stdbool.h>

#ifndef ATREE_H
#define ATREE_H

// Hodnota pretypovani
typedef enum{
    Integer2Double,
    Double2Integer,
    Double2Integer1
}TCast;

// Typ polozky ASS
typedef enum{
    at_operators,
    at_token,
    at_tsitem,
    at_type_cast
}ATType;

// Hodnota polozky ASS
// op_value Ulozeni hodnoty operatoru
// token Ulozeni tokenu
// tsItem Ulozeni ukazatele do TS
// type_cast Ulozeni hodnoty pretypovani
typedef union{
    Operators op_value;
    Token * token;
    Tsymtab_item * tsItem;
    TCast type_cast;
}ATAtribute;

// Struktura polozky ASS
// type Typ polozky
// Atr Hodnota polozky
typedef struct{
    ATType type;
    ATAtribute Atr;
}ATData;

// Struktura ASS
// processed znaci zda byl uzel jiz zpracovan
// data hodnota, kterou obsahuje uzel ASS
// left ukazatel na levou vetev ASS
// right ukazatel na pravou vetev ASS
typedef struct atleaf{
    bool processed;
    ATData data;
    struct atleaf *left;
    struct atleaf *right;
}ATLeaf;

/**
 * Vytvori novy uzel ASS
 * @param  data hodnota, ktera je prirazena novemu uzlu ASS
 * @return      ukazatel na novy uzel ASS
 */
ATLeaf *make_leaf(ATData data);

/**
 * Navaze na sebe jednotlive uzly ASS 
 * @param  leaf_1 navaze uzel jako levou vetev
 * @param  leaf_2 navaze uzel jako pravou vetev
 * @param  data   hodnota, ktera je prirazena korenovemu uzlu
 * @return        vrati ukazatel na nove vytvoreny korenovy uzel
 */
ATLeaf *make_tree(ATLeaf *leaf_1, ATLeaf *leaf_2, ATData data);

/**
 * Postupne uvolni cely ASS
 * @param leaf ukazatale na ASS
 */
void dispose_at(ATLeaf *leaf);
#endif
