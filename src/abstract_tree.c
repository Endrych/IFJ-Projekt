#include "token.h"
#include "abstract_tree.h"
#include <stdio.h>
#include <stdlib.h>

Abs_tree *at_init(){
    Abs_tree *tree;
    tree = (Abs_tree *)malloc(sizeof(struct atleaf));
    if(tree == NULL){
        fprintf(stderr, "Problem with memory");
        EXIT_FAILURE;
    }
    tree->first->left = NULL;
    tree->first->right = NULL;
    return tree;
}

ATLeaf *make_leaf(ATData data){
    ATLeaf *new_leaf = (ATLeaf *) malloc(sizeof(struct atleaf));
    new_leaf->data = data;
    new_leaf->left = NULL;
    new_leaf->right = NULL;
    
    return new_leaf;
}

void make_tree(){}

