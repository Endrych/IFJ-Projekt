#include <stdio.h>
#include "../src/generator.h"
#include <stdlib.h>
#include "../src/gen_stacks.h"

int main(){
    open_output();
    ATLeaf *leaf;
    ATLeaf *leaf2;
    ATLeaf *leaf3;
    ATLeaf *leaf4;
    ATLeaf *tree;
    ATLeaf *tree2;
    ATLeaf *tree3;
    ATData data;
    ATData data2;    
    ATData data3; 
    ATData data4;  
    ATData data5;   
    ATData data6;     
   	Token* token = create_token();
   	Token* token2 = create_token();
   	Token* token3 = create_token();
   	Token* token4 = create_token();
       
       
    printf("@@\n");

    token->atribute.double_value = 7;
    token->type = type_double;
    token2->atribute.double_value = 2; 
    token2->type = type_double;
    data.Atr.token = token;
    data2.Atr.token = token2;
    data.type = at_token;
    data2.type = at_token;    
    leaf = make_leaf(data);
    leaf2 = make_leaf(data2);
    data3.type = at_operators;
    data3.Atr.op_value = op_mul;
    tree = make_tree(leaf, leaf2, data3);
    // generate_expression(tree);
    generate_expression(tree);

    // token3->atribute.int_value = 5;
    // token3->type = type_integer;
    // data3.Atr.token = token3;
    // data3.type = at_token;
    // token4->atribute.int_value = 8;
    // token4->type = type_integer;
    // data6.Atr.token = token4;
    // data6.type = at_token;
    // leaf3 = make_leaf(data3);
    // leaf4 = make_leaf(data6)  ; 
    // data4.type = at_operators;
    // data4.Atr.op_value = op_sub;
    // tree2 = make_tree(leaf3, leaf4, data4);
    // data5.type = at_operators;
    // data5.Atr.op_value = op_add;
    // tree3 = make_tree(tree, tree2, data5);
    // to_print(tree3, 1);
    generate_expression(tree3);

    return 0;
}