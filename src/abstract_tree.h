#include "token.h"

typedef union{
    Operators op_value;
    int int_value;
    float float_value;
}ATData;

typedef struct atleaf{
    ATData data;
    struct atleaf *left;
    struct atleaf *right;
}ATLeaf;

ATLeaf *make_leaf(ATData data);
ATLeaf *make_tree(ATLeaf *leaf_1, ATLeaf *leaf_2, ATData data);
void free_at(ATLeaf *leaf);    