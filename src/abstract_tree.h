#include "token.h"

#ifndef ATREE_H
#define ATREE_H


typedef union{
    Operators op_value;
    Token * token;
}ATAtribute;

typedef struct{
    Type type;
    ATAtribute Atr;
}ATData;

typedef struct atleaf{
    ATData data;
    struct atleaf *left;
    struct atleaf *right;
}ATLeaf;

ATLeaf *make_leaf(ATData data);
ATLeaf *make_tree(ATLeaf *leaf_1, ATLeaf *leaf_2, ATData data);
void dispose_at(ATLeaf *leaf);    
#endif