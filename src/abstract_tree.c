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

ATLeaf *make_tree(ATLeaf *leaf_1, ATLeaf *leaf_2, ATData data){
    ATLeaf *new_tree = (ATLeaf *) malloc(sizeof(struct atleaf));
    new_tree->data = data;
    new_tree->left = leaf_1;
    new_tree->right = leaf_2;    

    return new_tree;
}

