#include "token.h"
#include "abstract_tree.h"
#include <stdio.h>
#include <stdlib.h>

Abs_tree *at_create(){
    Abs_tree *tree;
    tree = (Abs_tree *)malloc(sizeof(struct atleaf));
    if(tree == NULL){
        fprintf(stderr, "Problem with memory");
        EXIT_FAILURE;
    }
    return tree;
}

void at_init(Abs_tree *tree){
    tree->first->left = NULL;
    tree->first->right = NULL;
}