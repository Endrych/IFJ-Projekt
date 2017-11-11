#include <stdio.h>
#include "../src/abstract_tree.h"
#include "../src/token.h"


int main(){
    printf("Wow@)");
    ATLeaf *leaf;
    ATLeaf *leaf2;
    ATLeaf *tree;
    ATData data;
    ATData data3;    
    data.int_value = 1;
    data3.int_value = 2;
    leaf = make_leaf(data);
    leaf2 = make_leaf(data3);
    ATData data2;
    data2.int_value = 3;
    tree = make_tree(leaf, leaf2, data2);
    printf("\nHOTOVO: %d\n", leaf->data.int_value);
    printf("\nHOTOVO: %d\n", tree->data.int_value);
    printf("\nHOTOVO: %d\n", tree->left->data);
    printf("\nHOTOVO: %d\n", tree->right->data);
    
    

    return 0;
}