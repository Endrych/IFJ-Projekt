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
// processed
// data
// left
// right
typedef struct atleaf{
    bool processed;
    ATData data;
    struct atleaf *left;
    struct atleaf *right;
}ATLeaf;

/**
 * [make_leaf description]
 * @param  data [description]
 * @return      [description]
 */
ATLeaf *make_leaf(ATData data);

/**
 * [make_tree description]
 * @param  leaf_1 [description]
 * @param  leaf_2 [description]
 * @param  data   [description]
 * @return        [description]
 */
ATLeaf *make_tree(ATLeaf *leaf_1, ATLeaf *leaf_2, ATData data);

/**
 * [dispose_at description]
 * @param leaf [description]
 */
void dispose_at(ATLeaf *leaf);
#endif
