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

void make_leaf(Abs_tree *tree, ATData data){
    ATLeaf *leaf = (ATLeaf *) malloc(sizeof(struct atleaf));
    leaf->data = data;
    leaf->left = NULL;
    leaf->right = NULL;
    
}

void make_tree(){}

