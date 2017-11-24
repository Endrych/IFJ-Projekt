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

typedef enum{
    Integer2Double,
    Double2Integer
}TCast;


typedef enum{
    at_operators,
    at_token,
    at_tsitem,
    at_type_cast
}ATType;

typedef union{
    Operators op_value;
    Token * token;
    Tsymtab_item * tsItem;
    TCast type_cast;
}ATAtribute;

typedef struct{
    ATType type;
    ATAtribute Atr;
}ATData;

typedef struct atleaf{
    bool processed;
    ATData data;
    struct atleaf *left;
    struct atleaf *right;
}ATLeaf;

ATLeaf *make_leaf(ATData data);
ATLeaf *make_tree(ATLeaf *leaf_1, ATLeaf *leaf_2, ATData data);
void dispose_at(ATLeaf *leaf);    
#endif