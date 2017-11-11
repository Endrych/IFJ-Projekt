#include <stdio.h>
#include "../src/abstract_tree.h"
#include "../src/token.h"

// druhy argument znaci jestli se jedna o uzel na leve nebo
//prave strane nebo o korenovy uzel 
//1.korenovy, 2.levy, 3.pravy
//je zde vyuzit pruchod in order 
void to_print(ATLeaf *leaf, int value){
    if(leaf->left != NULL){
        to_print(leaf->left, 2);
    }
    if(leaf->right != NULL){
        to_print(leaf->right, 3);
    }
    if(value == 1){
        printf("|%d|\n",leaf->data.int_value);        
    }else if(value == 2){
        printf("/%d/\n",leaf->data.int_value);                
    }else{
        printf("\\%d\\\n",leaf->data.int_value);                
    }
}

int main(){
    ATLeaf *leaf;
    ATLeaf *leaf2;
    ATLeaf *tree;
    ATData data;
    ATData data2;    
    ATData data3;    
    data.int_value = 1;
    data2.int_value = 2;    
    data3.int_value = 3;
    leaf = make_leaf(data);
    //1. Leaf creation
    if(leaf == NULL ){
        printf("1. Leaf wasnt created\n");
    }else if(leaf->data.int_value != 1){
        printf("1. Wrong value assigned\n");
    }else{
        printf("1. SUCESS\n");
    }
    //2.tree creation
    leaf2 = make_leaf(data2);
    tree = make_tree(leaf, leaf2, data3);
    if(tree == NULL ){
        printf("2. Leaf wasnt created\n");
    }else if(tree->data.int_value != 3){
        printf("2. Wrong value assigned\n");
    }else if((tree->left == NULL)||(tree->right == NULL)){
        printf("2. Branches between leaves werent created properly\n");
    }else{
        printf("2. SUCESS\n");
    }
    //3.bigger tree test
    //je vyuzit pruchod in order tedy vysledek by mel byt
    //8 4 11 13 1 3 2 
    // / \ a | urcuji orientace jestli se jedna o levy,pravy 
    //nebo korenovy uzel
    data.int_value = 11;
    data3.int_value = 13;
    leaf = make_leaf(data);
    tree = make_tree(leaf, tree, data3);
    data.int_value = 8;
    data3.int_value = 4;
    leaf = make_leaf(data);
    tree = make_tree(leaf, tree, data3);
    to_print(tree, 1);
    //4.dispose tree checking for memory leaks
    dispose_at(tree);

    return 0;
}