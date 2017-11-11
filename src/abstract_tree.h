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

typedef struct{
    struct atleaf *first;
}Abs_tree;

